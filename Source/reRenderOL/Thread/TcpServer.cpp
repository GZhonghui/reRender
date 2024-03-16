#include "TcpServer.h"

Skybox Sky;

TcpServer::TcpServer()
{
    srand((uint32_t)time(nullptr));
    int Port = (rand() % 1000) + 10000;

    if(Server.listen(QHostAddress::Any, Port))
    {
        QObject::connect(&Server, &QTcpServer::newConnection, this, &TcpServer::OnConnect);

        Out::Log(pType::MESSAGE, "Bind Address at %d", Port);
    }else
    {
        Out::Log(pType::CRASH, "Bind Address Failed");
    }

    Sky.LoadFile(".\\Asset\\Skybox\\Creek");
}

TcpServer::~TcpServer()
{
    for(auto RenderThread : RenderThreads)
    {
        RenderThread->quit();
        RenderThread->wait();

        delete RenderThread;
    }

    RenderThreads.clear();
}

void TcpServer::OnConnect()
{
    if(Client) return;

    Client = Server.nextPendingConnection();
    QObject::connect(Client, &QTcpSocket::readyRead, this, &TcpServer::OnReadyRead);
    QObject::connect(Client, &QTcpSocket::stateChanged, this, &TcpServer::OnStateChanged);

    Out::Log(pType::MESSAGE, "Got Client");
}

void TcpServer::OnReadyRead()
{
    QTcpSocket* Sender = static_cast<QTcpSocket*>(QObject::sender());
    if(Sender && Sender == Client)
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
            case NMType::CheckRequest:
            {
                PCheckRequest CheckRequest;
                bool Ret = CheckRequest.ParseFromArray(Message.MessageBuffer.data(), Message.MessageLen);
                
                if(Ret)
                {
                    Out::Log(pType::MESSAGE, "Recved Check");
                    Out::Log(pType::MESSAGE, "Core = %d, Spp = %d", CheckRequest.core(), CheckRequest.spp());
                    Out::Log(pType::MESSAGE, "Width = %d, Height = %d", CheckRequest.width(), CheckRequest.height());

                    RenderWidth = CheckRequest.width();
                    RenderHeight = CheckRequest.height();
                    Core = CheckRequest.core();
                    Spp = CheckRequest.spp();
                }else
                {
                    Out::Log(pType::CRASH, "Cant Parse Check Info");
                }
            }break;

            case NMType::SceneRequest:
            {
                PSceneRequest SceneRequest;
                bool Ret = SceneRequest.ParseFromArray(Message.MessageBuffer.data(), Message.MessageLen);

                if(Ret)
                {
                    SceneData = SceneRequest; // Save the Scene Data

                    Out::Log(pType::MESSAGE, "Recved Scene Data");

                    Out::Log(pType::MESSAGE, "Material Count = %d", SceneRequest.materials_size());
                    Out::Log(pType::MESSAGE, "Mesh Count = %d", SceneRequest.meshs_size());

                }else
                {
                    Out::Log(pType::CRASH, "Cant Parse Scene Data");
                }
            }break;

            case NMType::StartRenderRequest:
            {
                Out::Log(pType::MESSAGE, "Recved Start Render");

                if(!RenderThreads.size())
                {
                    Out::Log(pType::MESSAGE, "Initing Render Core");

                    int TotalPixel = RenderWidth * RenderHeight;
                    int ThreadBlock = (int)(TotalPixel / Core) + ((TotalPixel % Core) ? 1 : 0);

                    int L = 1, R = ThreadBlock;

                    for(int i = 1; i <= Core; i += 1)
                    {
                        if(L > R) break;

                        auto RenderThread = new ServerProcessor;
                        RenderThread->InitRenderCore(RenderWidth, RenderHeight, Spp, L, R, &SceneData);

                        QObject::connect(RenderThread, &ServerProcessor::RenderedPixel, this, &TcpServer::OnRenderedPixel);

                        RenderThread->setStackSize(256 * 1024 * 1024);
                        
                        RenderThread->start();

                        RenderThreads.push_back(RenderThread);

                        L = R + 1;
                        R = std::min(R + ThreadBlock, TotalPixel);
                    }
                }
            }break;

            default:
            {
                Out::Log(pType::CRASH, "Unknown Message Type");
            }break;
            }
        }
    }
}

void TcpServer::OnStateChanged(QAbstractSocket::SocketState SocketState)
{
    if (SocketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* Sender = static_cast<QTcpSocket*>(QObject::sender());
        if(Sender == Client)
        {
            Client = nullptr;

            Out::Log(pType::MESSAGE, "Client Left");

            exit(0);
        }
    }
}

void TcpServer::OnRenderedPixel(int X, int Y, QVector3D Color)
{
    UniformMessage Message;
    Message.MessageType = static_cast<int>(NMType::RenderPixelResponse);

    PRenderPixelResponse RenderPixelResponse;

    auto RenderResult = new Vec3;
    RenderResult->set_x(Color.x());
    RenderResult->set_y(Color.y());
    RenderResult->set_z(Color.z());

    RenderPixelResponse.set_x(X);
    RenderPixelResponse.set_y(Y);
    RenderPixelResponse.set_allocated_result(RenderResult);

    Message.MessageLen = RenderPixelResponse.ByteSizeLong();
    Message.MessageBuffer.resize(Message.MessageLen);
    RenderPixelResponse.SerializeToArray(Message.MessageBuffer.data(), Message.MessageLen);

    std::vector<uint8_t> SendBuffer;

    MessageToBytes(Message, SendBuffer);

    if(Client)
    {
        int TotalSize = SendBuffer.size();
        int SendedSize = 0;
        char* Buffer = (char*)SendBuffer.data();

        for(int i=0;i<128;i+=1)
        {
            if(SendedSize >= TotalSize)
            {
                break;
            }
            SendedSize += Client->write(Buffer + SendedSize, TotalSize - SendedSize);
        }
    }
}