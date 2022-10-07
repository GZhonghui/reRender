#include "RunTarget.h"

RunTarget::RunTarget(int ID, int Width, int Height, PSceneRequest* SceneData, QWidget *parent)
    : SceneData(SceneData), QWidget{parent}
{
    TargetID = ID;

    RenderWidth = Width;
    RenderHeight = Height;

    RecvedRenderResult = std::make_unique<float[]>(Width * Height * 3);

    auto MainLayout = new QVBoxLayout;
    setLayout(MainLayout);

    {
        auto HLayout = new QHBoxLayout;
        MainLayout->addLayout(HLayout);

        HLayout->addWidget(new QLabel("IP Address:"));

        InputIp = new QLineEdit;
        InputIp->setText("127.0.0.1");
        HLayout->addWidget(InputIp);

        HLayout->addWidget(new QLabel("Port:"));

        InputPort = new QLineEdit;
        InputPort->setText("0");
        HLayout->addWidget(InputPort);
    }

    {
        auto HLayout = new QHBoxLayout;
        MainLayout->addLayout(HLayout);

        HLayout->addWidget(new QLabel("Core Number:"));

        InputCore = new QLineEdit;
        InputCore->setText("1");
        HLayout->addWidget(InputCore);

        HLayout->addWidget(new QLabel("SPP:"));

        InputSpp = new QLineEdit;
        InputSpp->setText("2");
        HLayout->addWidget(InputSpp);
    }

    {
        auto HLayout = new QHBoxLayout;
        MainLayout->addLayout(HLayout);

        auto IdLabel = new QLabel(QString("[%1]").arg(QString::number(TargetID)));
        HLayout->addWidget(IdLabel);

        ConnectionStatus = new QLabel("Waiting");
        HLayout->addWidget(ConnectionStatus);

        SpendTime = new QLabel("");
        SpendTime->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        HLayout->addWidget(SpendTime);

        auto ConnectButton = new QPushButton("Connect");
        ConnectButton->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        QObject::connect(ConnectButton, &QPushButton::clicked, this, &RunTarget::PushConnectButton);
        HLayout->addWidget(ConnectButton);

        auto RemoveButton = new QPushButton("Remove");
        RemoveButton->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        QObject::connect(RemoveButton, &QPushButton::clicked, this, &RunTarget::PushRemoveButton);
        HLayout->addWidget(RemoveButton);
    }

    {
        RenderProgress = new QProgressBar;
        RenderProgress->setRange(0,RenderWidth * RenderHeight);
        RenderProgress->setValue(0);
        MainLayout->addWidget(RenderProgress);
    }
}

RunTarget::~RunTarget()
{
    if(Server)
    {
        Server->close();

        delete Server;
        Server = nullptr;
    }
}

void RunTarget::PushConnectButton()
{
    ServerIp = InputIp->text();
    ServerPort = InputPort->text().toInt();
    ServerCore = InputCore->text().toInt();
    ServerSpp = InputSpp->text().toInt();

    if(Server)
    {
        Server->close();

        delete Server;
        Server = nullptr;
    }

    Server = new QTcpSocket;
    Server->connectToHost(ServerIp, ServerPort);

    if(Server->waitForConnected(1000))
    {
        ConnectionStatus->setText("Ready");
        Connected = true;

        QObject::connect(Server, &QTcpSocket::readyRead, this, &RunTarget::OnReadyRead);

        {
            UniformMessage CheckMessage;

            PCheckRequest CheckRequest;
            CheckRequest.set_core(ServerCore);
            CheckRequest.set_spp(ServerSpp);

            CheckRequest.set_width(RenderWidth);
            CheckRequest.set_height(RenderHeight);

            CheckMessage.MessageType = static_cast<int>(NMType::CheckRequest);
            CheckMessage.MessageLen = CheckRequest.ByteSizeLong();
            CheckMessage.MessageBuffer.resize(CheckMessage.MessageLen);
            CheckRequest.SerializeToArray(CheckMessage.MessageBuffer.data(), CheckMessage.MessageLen);
            
            std::vector<uint8_t> SendBuffer;
            MessageToBytes(CheckMessage, SendBuffer);

            Server->write((char*)SendBuffer.data(), SendBuffer.size());
        }
        
        {
            UniformMessage SceneMessage;

            SceneMessage.MessageType = static_cast<int>(NMType::SceneRequest);
            SceneMessage.MessageLen = SceneData->ByteSizeLong();
            SceneMessage.MessageBuffer.resize(SceneMessage.MessageLen);
            SceneData->SerializeToArray(SceneMessage.MessageBuffer.data(), SceneMessage.MessageLen);
            
            std::vector<uint8_t> SendBuffer;
            MessageToBytes(SceneMessage, SendBuffer);

            Server->write((char*)SendBuffer.data(), SendBuffer.size());
        }
        
    }
    else
    {
        ConnectionStatus->setText("Failed");
        Connected = false;
        
        delete Server;
        Server = nullptr;
    }
}

void RunTarget::PushRemoveButton()
{
    if(TargetID)
    {
        RemoveSelf(TargetID);
    }
}

void RunTarget::OnReadyRead()
{
    QTcpSocket* Sender = static_cast<QTcpSocket*>(QObject::sender());
    if(Sender && Sender == Server)
    {
        QByteArray Datas = Sender->readAll();

        int ThisReadedSize = Datas.size();
        int BeforeReadedSize = ReadedBuffer.size();

        ReadedBuffer.resize(BeforeReadedSize + ThisReadedSize);
        memcpy(ReadedBuffer.data() + BeforeReadedSize, Datas.data(), ThisReadedSize);
        
        UniformMessage Message;
        while(BytesToMessage(ReadedBuffer, Message))
        {
            switch(static_cast<NMType>(Message.MessageType))
            {
            case NMType::RenderPixelResponse:
            {
                PRenderPixelResponse RenderPixelResponse;
                bool Ret = RenderPixelResponse.ParseFromArray(Message.MessageBuffer.data(), Message.MessageLen);
                
                if(Ret)
                {
                    RecvedCount += 1;
                    RenderProgress->setValue(RecvedCount);

                    int X = RenderPixelResponse.x();
                    int Y = RenderPixelResponse.y();
                    int PixelIndex = (X * RenderWidth + Y) * 3;

                    auto Result = RenderPixelResponse.result();
                    RecvedRenderResult[PixelIndex + 0] = Result.x();
                    RecvedRenderResult[PixelIndex + 1] = Result.y();
                    RecvedRenderResult[PixelIndex + 2] = Result.z();

                    if(GetComplated())
                    {
                        ConnectionStatus->setText("Complated");
                        RenderFinish(RecvedRenderResult.get(), ServerSpp);
                    }
                }
            }break;

            default:
            {

            }break;
            }
        }
    }
}

void RunTarget::StartRender()
{
    if(!Connected || !Server) return;

    RecvedCount = 0;

    UniformMessage StartRenderMessage;
    StartRenderMessage.MessageType = static_cast<int>(NMType::StartRenderRequest);
    StartRenderMessage.MessageLen = 0;

    std::vector<uint8_t> SendBuffer;
    MessageToBytes(StartRenderMessage, SendBuffer);

    int TotalSize = SendBuffer.size();
    int SendedSize = 0;
    char* Buffer = (char*)SendBuffer.data();

    for(int i=0;i<128;i+=1)
    {
        if(SendedSize >= TotalSize)
        {
            break;
        }
        SendedSize += Server->write(Buffer + SendedSize, TotalSize - SendedSize);
    }
    

    ConnectionStatus->setText("Rendering");
}