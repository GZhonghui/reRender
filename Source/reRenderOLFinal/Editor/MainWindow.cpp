#include "MainWindow.h"

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

#include <OpenImageDenoise/config.h>
#include <OpenImageDenoise/oidn.hpp>

class deNoiser
{
public:
	deNoiser()
	{
		inMap = nullptr;
		colorMap = nullptr;
		normalMap = nullptr;
	}
	~deNoiser() = default;

public:
	float* inMap;
	float* colorMap;
	float* normalMap;
	int imageWidth;
	int imageHeight;
	//Default 3 channels

public:
	bool deNoise(uint8_t* outImage)
	{
		if (!inMap) return false;

		int BufferSize = imageWidth * imageHeight * 3;
		std::unique_ptr<float[]> resImage = std::make_unique<float[]>(BufferSize);

		oidn::DeviceRef Device = oidn::newDevice();
		Device.commit();

		oidn::FilterRef Filter = Device.newFilter("RT");
		Filter.setImage("color", inMap, oidn::Format::Float3, imageWidth, imageHeight);
		Filter.setImage("output", resImage.get(), oidn::Format::Float3, imageWidth, imageHeight);

		if (colorMap)
		{
			Filter.setImage("albedo", colorMap, oidn::Format::Float3, imageWidth, imageHeight);
		}
		if (normalMap)
		{
			Filter.setImage("normal", normalMap, oidn::Format::Float3, imageWidth, imageHeight);
		}

		Filter.commit();

		Filter.execute();

		//outImage is not created in This Function
		for (int i = 0; i < BufferSize; i += 1)
		{
			outImage[i] = static_cast<uint8_t>(resImage[i] * 255.999);
		}
		return true;
	}
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
    this->setWindowIcon(QPixmap(":/Image/Logo.png"));
    this->setMinimumSize(1200, 800);

    this->LoadStyle(":/Qss/psblack.css");
    this->BuildUI();
}

MainWindow::~MainWindow() {}

void MainWindow::BuildUI()
{
    // ToolBar
    {
        auto ToolBar = this->addToolBar("Tools");
        ToolBar->setFloatable(false);
        ToolBar->setMovable(false);

        /*
        auto SaveButton = new QPushButton("Save");
        QObject::connect(SaveButton, &QPushButton::clicked, this, &MainWindow::PushSave);
        ToolBar->addWidget(SaveButton);

        auto OpenButton = new QPushButton("Open");
        QObject::connect(OpenButton, &QPushButton::clicked, this, &MainWindow::PushOpen);
        ToolBar->addWidget(OpenButton);
        */

        InputMeshName = new QLineEdit;
        InputMeshName->setMaximumWidth(256);
        ToolBar->addWidget(InputMeshName);

        auto LoadButton = new QPushButton("Load");
        QObject::connect(LoadButton, &QPushButton::clicked, this, &MainWindow::PushLoad);
        ToolBar->addWidget(LoadButton);

        auto SphereButton = new QPushButton("Sphere");
        QObject::connect(SphereButton, &QPushButton::clicked, this, &MainWindow::PushSphere);
        ToolBar->addWidget(SphereButton);

        auto DeleteButton = new QPushButton("Delete");
        QObject::connect(DeleteButton, &QPushButton::clicked, this, &MainWindow::PushDelete);
        ToolBar->addWidget(DeleteButton);

        auto ModeButton = new QPushButton("Mode");
        QObject::connect(ModeButton, &QPushButton::clicked, this, &MainWindow::PushMode);
        ToolBar->addWidget(ModeButton);

        auto RenderButton = new QPushButton("Render");
        QObject::connect(RenderButton, &QPushButton::clicked, this, &MainWindow::PushRender);
        ToolBar->addWidget(RenderButton);

        auto DenoiseButton = new QPushButton("Denoise");
        QObject::connect(DenoiseButton, &QPushButton::clicked, this, &MainWindow::PushDenoise);
        ToolBar->addWidget(DenoiseButton);

        auto ExportButton = new QPushButton("Export"); // Save Image
        QObject::connect(ExportButton, &QPushButton::clicked, this, &MainWindow::PushExport);
        ToolBar->addWidget(ExportButton);

        auto AboutButton = new QPushButton("About");
        QObject::connect(AboutButton, &QPushButton::clicked, this, &MainWindow::PushAbout);
        ToolBar->addWidget(AboutButton);
    }

    auto MainWidget = new QWidget;
    auto MainLayout = new QGridLayout;

    MainWidget->setLayout(MainLayout);
    this->setCentralWidget(MainWidget);

    // Left Scene
    {
        auto Scenes = new QTabWidget;
        Scenes->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        MainLayout->addWidget(Scenes, 0, 0, 2, 1);

        // Preview Scene
        {
            RenderPreview = new GlView;
            RenderPreview->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
            Scenes->addTab(RenderPreview, "Preview");

            RenderPreview->setFocusPolicy(Qt::ClickFocus);

            QObject::connect(this, &MainWindow::LoadMesh, RenderPreview, &GlView::LoadMesh);
        }

        // Render Scene
        {
            RenderResult = new QLabel;
            RenderResult->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
            RenderResult->setAlignment(Qt::AlignCenter);
            Scenes->addTab(RenderResult, "Render");
        }
    }

    // Right Panel
    {
        const int RightWidth = 360;

        // List
        {
            ObjectList = new QListWidget;
            ObjectList->setMinimumWidth(RightWidth);
            ObjectList->setMaximumWidth(RightWidth);
            ObjectList->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
            MainLayout->addWidget(ObjectList, 0, 1, 1, 1);

            QObject::connect(ObjectList, &QListWidget::currentTextChanged, this, &MainWindow::SelectedObjectChanged);
        }

        // Settings
        {
            auto Details = new QTabWidget;
            Details->setMinimumWidth(RightWidth);
            Details->setMaximumWidth(RightWidth);
            Details->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
            MainLayout->addWidget(Details, 1, 1, 1, 1);

            // Object
            {
                auto DetailObject = new QWidget;
                Details->addTab(DetailObject, "Object");

                auto ObjectLayout = new QVBoxLayout;
                DetailObject->setLayout(ObjectLayout);
                
                ObjectLayout->addWidget(new QLabel("Location"));
                {
                    auto Layout = new QHBoxLayout;
                    ObjectLayout->addLayout(Layout);
                    
                    for(int i=0; i<3; i+=1)
                    {
                        ObjectLocation[i] = new QDoubleSpinBox;
                        ObjectLocation[i]->setRange(-50.0, 50.0);
                        Layout->addWidget(ObjectLocation[i]);
                        QObject::connect(ObjectLocation[i], QDoubleSpinBox::valueChanged, this, MainWindow::UpdateTransform);
                    }
                }

                ObjectLayout->addWidget(new QLabel("Rotation"));
                {
                    auto Layout = new QHBoxLayout;
                    ObjectLayout->addLayout(Layout);
                    
                    for(int i=0; i<3; i+=1)
                    {
                        ObjectRotation[i] = new QDoubleSpinBox;
                        ObjectRotation[i]->setRange(-180.0, 180.0);
                        Layout->addWidget(ObjectRotation[i]);
                        QObject::connect(ObjectRotation[i], QDoubleSpinBox::valueChanged, this, MainWindow::UpdateTransform);
                    }
                }

                ObjectLayout->addWidget(new QLabel("Scale"));
                {
                    auto Layout = new QHBoxLayout;
                    ObjectLayout->addLayout(Layout);
                    
                    for(int i=0; i<3; i+=1)
                    {
                        ObjectScale[i] = new QDoubleSpinBox;
                        ObjectScale[i]->setRange(0.1, 30.0);
                        Layout->addWidget(ObjectScale[i]);
                        QObject::connect(ObjectScale[i], QDoubleSpinBox::valueChanged, this, MainWindow::UpdateTransform);
                    }
                }

                ObjectLayout->addWidget(new QLabel("Material"));
                {
                    ObjectMaterial = new QComboBox;
                    ObjectLayout->addWidget(ObjectMaterial);

                    QObject::connect(ObjectMaterial, &QComboBox::currentTextChanged, this, &MainWindow::AttachMaterial);
                }

                ObjectLayout->addStretch();
            }

            // Material
            {
                auto DetailMaterial = new QWidget;
                Details->addTab(DetailMaterial, "Material");

                auto MaterialLayout = new QVBoxLayout;
                DetailMaterial->setLayout(MaterialLayout);

                {
                    auto Layout = new QHBoxLayout;
                    MaterialLayout->addLayout(Layout);

                    {
                        InputMaterialName = new QLineEdit;
                        Layout->addWidget(InputMaterialName);
                    }

                    {
                        auto Button = new QPushButton("Add");
                        Layout->addWidget(Button);

                        QObject::connect(Button, &QPushButton::clicked, this, &MainWindow::PushAddMaterial);
                    }

                    {
                        auto Button = new QPushButton("Delete");
                        Button->setDisabled(true);
                        Layout->addWidget(Button);
                    }

                }
                
                {
                    auto Layout = new QHBoxLayout;
                    MaterialLayout->addLayout(Layout);

                    SelectMaterial = new QComboBox;
                    SelectMaterial->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
                    Layout->addWidget(SelectMaterial);
                    QObject::connect(SelectMaterial, &QComboBox::currentTextChanged, this, MainWindow::SelectedMaterialChanged);

                    auto SaveButton = new QPushButton("Save");
                    SaveButton->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
                    Layout->addWidget(SaveButton);

                    QObject::connect(SaveButton, &QPushButton::clicked, this, &MainWindow::UpdateMaterialParameter);
                }

                SelectMaterialType = new QComboBox;
                SelectMaterialType->addItem("Diffuse");
                SelectMaterialType->addItem("Metal");
                SelectMaterialType->addItem("Glass");
                SelectMaterialType->addItem("Emit");
                SelectMaterialType->addItem("Fog");
                MaterialLayout->addWidget(SelectMaterialType);
                QObject::connect(SelectMaterialType, &QComboBox::currentTextChanged, this, &MainWindow::SelectedMaterialTypeChanged);

                MatParameter = new QStackedWidget;
                MaterialLayout->addWidget(MatParameter);

                {
                    MatDiffuseParameter = new QWidget;
                    MatParameter->addWidget(MatDiffuseParameter);
                    auto Layout = new QVBoxLayout;
                    MatDiffuseParameter->setLayout(Layout);

                    auto ColorButton = new QPushButton("Color");
                    QObject::connect(ColorButton, &QPushButton::clicked, this, &MainWindow::SelectDiffuseColor);

                    Layout->addWidget(ColorButton);
                    Layout->addStretch();
                }

                {
                    MatMetalParameter = new QWidget;
                    MatParameter->addWidget(MatMetalParameter);
                    auto Layout = new QVBoxLayout;
                    MatMetalParameter->setLayout(Layout);

                    auto ColorButton = new QPushButton("Color");
                    QObject::connect(ColorButton, &QPushButton::clicked, this, &MainWindow::SelectMetalColor);

                    Layout->addWidget(ColorButton);

                    {
                        auto HLayout = new QHBoxLayout;
                        Layout->addLayout(HLayout);

                        HLayout->addWidget(new QLabel("Fuzzy:"));

                        MatMetalFuzzy = new QDoubleSpinBox;
                        MatMetalFuzzy->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
                        HLayout->addWidget(MatMetalFuzzy);
                    }

                    Layout->addStretch();
                }

                {
                    MatGlassParameter = new QWidget;
                    MatParameter->addWidget(MatGlassParameter);
                    auto Layout = new QVBoxLayout;
                    MatGlassParameter->setLayout(Layout);

                    {
                        auto HLayout = new QHBoxLayout;
                        Layout->addLayout(HLayout);

                        HLayout->addWidget(new QLabel("IOR:"));

                        MatGlassIOR = new QDoubleSpinBox;
                        MatGlassIOR->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
                        HLayout->addWidget(MatGlassIOR);
                    }

                    Layout->addStretch();
                }

                {
                    MatEmitParameter = new QWidget;
                    MatParameter->addWidget(MatEmitParameter);
                    auto Layout = new QVBoxLayout;
                    MatEmitParameter->setLayout(Layout);

                    auto ColorButton = new QPushButton("Color");
                    QObject::connect(ColorButton, &QPushButton::clicked, this, &MainWindow::SelectEmitColor);

                    Layout->addWidget(ColorButton);

                    {
                        auto HLayout = new QHBoxLayout;
                        Layout->addLayout(HLayout);

                        HLayout->addWidget(new QLabel("Intensity:"));

                        MatEmitIntensity = new QDoubleSpinBox;
                        MatEmitIntensity->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
                        HLayout->addWidget(MatEmitIntensity);
                    }

                    Layout->addStretch();
                }

                {
                    MatFogParameter = new QWidget;
                    MatParameter->addWidget(MatFogParameter);
                }

                MaterialLayout->addStretch();
            }
        }
    }
}

void MainWindow::PushSave()
{

}

void MainWindow::PushOpen()
{

}

void MainWindow::PushLoad()
{
    auto MeshName = InputMeshName->text();
    if(!MeshName.length() || ObjectsName.count(MeshName.toStdString()))
    {
        return;
    }

    auto MeshFileName = QFileDialog::getOpenFileName(this, "Load...", ".", "Mesh File (*.obj)");
    if(MeshFileName.length())
    {
        ObjectList->addItem(MeshName);
        ObjectsName.insert(MeshName.toStdString());
        LoadMesh(MeshName, MeshFileName);
    }
}

void MainWindow::PushSphere()
{
    auto MeshName = InputMeshName->text();
    if(!MeshName.length() || ObjectsName.count(MeshName.toStdString()))
    {
        return;
    }

    ObjectList->addItem(MeshName);
    ObjectsName.insert(MeshName.toStdString());
    RenderPreview->LoadSphere(MeshName);
}

void MainWindow::PushDelete()
{    
    auto Item = ObjectList->currentItem();
    if(Item)
    {
        if(RenderPreview->m_NameToObjectPtr.count(Item->text().toStdString()))
        {
            RenderPreview->SelectedObjectName = "";
            RenderPreview->m_NameToObjectPtr.erase(Item->text().toStdString());
        }
        if(ObjectsName.count(Item->text().toStdString()))
        {
            ObjectsName.erase(Item->text().toStdString());
        }
        ObjectList->removeItemWidget(Item);
        delete Item; // Safe ?

        RenderPreview->update();
    }
}

void MainWindow::PushMode()
{
    RenderPreview->LineMode = !RenderPreview->LineMode;
    RenderPreview->update();
}

void MainWindow::PushRender()
{
    auto RenderSize = RenderPreview->size();

    PSceneRequest SceneRequest;
    RenderPreview->PackageScene(SceneRequest);

    int Width = RenderSize.width();
    int Height = RenderSize.height();

    auto ResultBuffer =  std::make_unique<float[]>(Width * Height * 3);

    RenderDialog Render(ResultBuffer.get(), Width, Height, &SceneRequest);

    Render.exec();

    QImage RenderedImage(Width, Height, QImage::Format_RGB888);

    RenderResultWidth = Width;
    RenderResultHeight = Height;
    RenderResultBuffer.clear();
    RenderResultBuffer.resize(Width * Height * 3);

    int PixelIndex = 0;
    for(int i = 0; i < Height; i += 1)
    {
        for(int j = 0; j < Width; j += 1)
        {
            auto Color = QColor();

            auto Limit = [](int Value)->int
            {
                return std::max(std::min(Value, 255), 0);
            };

            // RenderResultBuffer.push_back(Limit(ResultBuffer[PixelIndex + 0] * 255.999));
            // RenderResultBuffer.push_back(Limit(ResultBuffer[PixelIndex + 1] * 255.999));
            // RenderResultBuffer.push_back(Limit(ResultBuffer[PixelIndex + 2] * 255.999));

            int Stb_Index = (Width * (Height - i - 1) + j) * 3;
            RenderResultBuffer[Stb_Index + 0] = Limit(ResultBuffer[PixelIndex + 0] * 255.999);
            RenderResultBuffer[Stb_Index + 1] = Limit(ResultBuffer[PixelIndex + 1] * 255.999);
            RenderResultBuffer[Stb_Index + 2] = Limit(ResultBuffer[PixelIndex + 2] * 255.999);

            Color.setRed(Limit(ResultBuffer[PixelIndex + 0] * 255.999));
            Color.setGreen(Limit(ResultBuffer[PixelIndex + 1] * 255.999));
            Color.setBlue(Limit(ResultBuffer[PixelIndex + 2] * 255.999));
            PixelIndex += 3;

            RenderedImage.setPixelColor(j, (Height - i - 1), Color);
        }
    }

    RenderResult->setPixmap(QPixmap::fromImage(RenderedImage));
}

void MainWindow::PushDenoise()
{
    if(RenderResultBuffer.size())
    {
        std::vector<float> FBuffer;
        FBuffer.resize(RenderResultWidth * RenderResultHeight * 3);

        std::vector<uint8_t> UBuffer;
        UBuffer.resize(RenderResultWidth * RenderResultHeight * 3);

        deNoiser Solver;
        for(int i=0;i<RenderResultWidth * RenderResultHeight * 3;i+=1)
        {
            FBuffer[i] = RenderResultBuffer[i] / 255.999;
        }

        Solver.imageWidth = RenderResultWidth;
        Solver.imageHeight = RenderResultHeight;
        Solver.inMap = FBuffer.data();

        Solver.deNoise(UBuffer.data());

        RenderResultBuffer = UBuffer;

        QImage RenderedImage(RenderResultWidth, RenderResultHeight, QImage::Format_RGB888);

        for(int i = 0; i < RenderResultHeight; i += 1)
        {
            for(int j = 0; j < RenderResultWidth; j += 1)
            {
                auto Color = QColor();

                int Stb_Index = (RenderResultWidth * (RenderResultHeight - i - 1) + j) * 3;

                Color.setRed(RenderResultBuffer[Stb_Index + 0]);
                Color.setGreen(RenderResultBuffer[Stb_Index + 1]);
                Color.setBlue(RenderResultBuffer[Stb_Index + 2]);

                RenderedImage.setPixelColor(j, (RenderResultHeight - i - 1), Color);
            }
        }

        RenderResult->setPixmap(QPixmap::fromImage(RenderedImage));
    }
}

void MainWindow::PushExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Image Files (*.png)"));

    Out::Log(pType::MESSAGE,"Save Path: %s", fileName.toStdString().c_str());

    if(RenderResultBuffer.size())
    {
        stbi_write_png(fileName.toStdString().c_str(), RenderResultWidth, RenderResultHeight, 3, RenderResultBuffer.data(), 0);
    }   
}

void MainWindow::PushAbout()
{
    QDialog AboutDialog(this);
    AboutDialog.setFixedSize(680,480);
    QVBoxLayout AboutLayout;

    QLabel LogoLabel;
    QPixmap Logo(":/Image/reRender.png");
    LogoLabel.setPixmap(Logo);
    LogoLabel.setAlignment(Qt::AlignCenter);
    AboutLayout.addWidget(&LogoLabel);

    AboutLayout.addStretch();

    QFont FontTitle;
    FontTitle.setFamily("MV Boli");
    FontTitle.setPixelSize(24);

    QFont FontText;
    FontText.setFamily("Times New Roman");
    FontText.setPixelSize(16);

    QLabel AboutTitle;
    AboutTitle.setFont(FontTitle);
    AboutTitle.setText("This is reRender: A Open Source Render Engine");
    AboutTitle.setAlignment(Qt::AlignCenter);
    AboutLayout.addWidget(&AboutTitle);

    QString AboutTextContent;
    AboutTextContent+="\n";
    AboutTextContent+="reRender v0.1 is a C++ Program. reRender v1.0 is a Python Module.\n";
    AboutTextContent+="And this is reRender v2.0, a Standalone Software with UI.\n";
    AboutTextContent+="This Engine is my undergraduate Graduation Design.\n";
    AboutTextContent+="I'm GZhonghui. I got my Bachelor's Degree from CQUPT, China, in 2022.\n";
    AboutTextContent+="My major is Digital Media Technology.\n";
    AboutTextContent+="Mail: mail@gzher.com. Github: github.com/GZhonghui.\n";

    QLabel AboutText;
    AboutText.setText(AboutTextContent);
    AboutText.setAlignment(Qt::AlignCenter);
    AboutText.setFont(FontText);
    AboutLayout.addWidget(&AboutText);

    AboutDialog.setLayout(&AboutLayout);
    AboutDialog.exec();
}

void MainWindow::UpdateTransform(double Value)
{
    Transform NewTransform;

    for(int i=0;i<3;i+=1)
    {
        NewTransform.Location[i] = ObjectLocation[i]->value();
        NewTransform.Rotation[i] = ObjectRotation[i]->value();
        NewTransform.Scale[i] = ObjectScale[i]->value();
    }
    
    RenderPreview->UpdateTransform(NewTransform);

    // Update back
    {
        auto NowTrans = RenderPreview->GetTransform();

        for(int i=0;i<3;i+=1)
        {
            ObjectLocation[i]->setValue(NowTrans.Location[i]);
            ObjectRotation[i]->setValue(NowTrans.Rotation[i]);
            ObjectScale[i]->setValue(NowTrans.Scale[i]);
        }
    }
}

void MainWindow::AttachMaterial(const QString &text)
{
    RenderPreview->AttachMaterial(text.toStdString());
}

void MainWindow::SelectedObjectChanged(const QString &currentText)
{
    RenderPreview->SelectedObjectName = currentText.toStdString();

    auto NowTrans = RenderPreview->GetTransform();
    auto NowMat = RenderPreview->GetMaterial();

    for(int i=0;i<3;i+=1)
    {
        ObjectLocation[i]->setValue(NowTrans.Location[i]);
        ObjectRotation[i]->setValue(NowTrans.Rotation[i]);
        ObjectScale[i]->setValue(NowTrans.Scale[i]);
    }

    if(MaterialsName.count(NowMat))
    {
        ObjectMaterial->setCurrentText(QString::fromUtf8(NowMat.c_str()));
    }else
    {
        auto SelectedMat = ObjectMaterial->currentText().toStdString();
        RenderPreview->AttachMaterial(SelectedMat);
    }

    RenderPreview->update();
}

void MainWindow::SelectDiffuseColor()
{
    QColorDialog ColorPicker;
    ColorPicker.setCurrentColor(DiffuseColor);
    if(ColorPicker.exec())
    {
        DiffuseColor = ColorPicker.selectedColor();
    }
}

void MainWindow::SelectMetalColor()
{
    QColorDialog ColorPicker;
    ColorPicker.setCurrentColor(MetalColor);
    if(ColorPicker.exec())
    {
        MetalColor = ColorPicker.selectedColor();
    }
}

void MainWindow::SelectEmitColor()
{
    QColorDialog ColorPicker;
    ColorPicker.setCurrentColor(EmitColor);
    if(ColorPicker.exec())
    {
        EmitColor = ColorPicker.selectedColor();
    }
}

void MainWindow::PushAddMaterial()
{
    QString MaterialName = InputMaterialName->text();
    if(MaterialName.length() && (!MaterialsName.count(MaterialName.toStdString())))
    {
        MaterialsName.insert(MaterialName.toStdString());

        RenderPreview->AddMaterial(MaterialName.toStdString());

        ObjectMaterial->addItem(MaterialName);
        SelectMaterial->addItem(MaterialName);
    }
}

void MainWindow::SelectedMaterialChanged(const QString &text)
{
    std::shared_ptr<GlMaterial> SelectedMat = RenderPreview->m_NameToMaterialPtr[text.toStdString()];
    switch(SelectedMat->GetType())
    {
    case GMType::DIFFUSE:
    {
        GlDiffuse* Mat = (GlDiffuse*)(SelectedMat.get());
        DiffuseColor.setRed((int)(Mat->BaseColor[0] * 255.999));
        DiffuseColor.setGreen((int)(Mat->BaseColor[1] * 255.999));
        DiffuseColor.setBlue((int)(Mat->BaseColor[2] * 255.999));
        SelectMaterialType->setCurrentText("Diffuse");
    }break;

    case GMType::METAL:
    {
        GlMetal* Mat = (GlMetal*)(SelectedMat.get());
        MetalColor.setRed((int)(Mat->BaseColor[0] * 255.999));
        MetalColor.setGreen((int)(Mat->BaseColor[1] * 255.999));
        MetalColor.setBlue((int)(Mat->BaseColor[2] * 255.999));
        MatMetalFuzzy->setValue(Mat->Fuzzy);
        SelectMaterialType->setCurrentText("Metal");
    }break;

    case GMType::GLASS:
    {
        GlGlass* Mat = (GlGlass*)(SelectedMat.get());
        MatGlassIOR->setValue(Mat->IOR);
        SelectMaterialType->setCurrentText("Glass");
    }break;

    case GMType::EMIT:
    {
        GlEmit* Mat = (GlEmit*)(SelectedMat.get());
        EmitColor.setRed((int)(Mat->BaseColor[0] * 255.999));
        EmitColor.setGreen((int)(Mat->BaseColor[1] * 255.999));
        EmitColor.setBlue((int)(Mat->BaseColor[2] * 255.999));
        MatEmitIntensity->setValue(Mat->Intensity);
        SelectMaterialType->setCurrentText("Emit");
    }break;

    case GMType::FOG:
    {
        GlFog* Mat = (GlFog*)(SelectedMat.get());
        SelectMaterialType->setCurrentText("Fog");
    }break;
    }
}

void MainWindow::SelectedMaterialTypeChanged(const QString &text)
{
    auto MatTypeStr = text.toStdString();
    if(MatTypeStr == "Diffuse")
    {
        MatParameter->setCurrentWidget(MatDiffuseParameter);
    }
    else if(MatTypeStr == "Metal")
    {
        MatParameter->setCurrentWidget(MatMetalParameter);
    }
    else if(MatTypeStr == "Glass")
    {
        MatParameter->setCurrentWidget(MatGlassParameter);
    }
    else if(MatTypeStr == "Emit")
    {
        MatParameter->setCurrentWidget(MatEmitParameter);
    }else if(MatTypeStr == "Fog")
    {
        MatParameter->setCurrentWidget(MatFogParameter);
    }

    RenderPreview->update();
}

void MainWindow::UpdateMaterialParameter()
{
    auto NowMatName = SelectMaterial->currentText().toStdString();
    if(!NowMatName.length()) return;

    auto MatTypeStr = SelectMaterialType->currentText().toStdString();
    if(MatTypeStr == "Diffuse")
    {
        auto NewMat = std::make_shared<GlDiffuse>();
        NewMat->Name = NowMatName;
        NewMat->BaseColor[0] = DiffuseColor.red() / 255.999;
        NewMat->BaseColor[1] = DiffuseColor.green() / 255.999;
        NewMat->BaseColor[2] = DiffuseColor.blue() / 255.999;

        RenderPreview->m_NameToMaterialPtr[NowMatName] = NewMat;
    }
    else if(MatTypeStr == "Metal")
    {
        auto NewMat = std::make_shared<GlMetal>();
        NewMat->Name = NowMatName;
        NewMat->BaseColor[0] = MetalColor.red() / 255.999;
        NewMat->BaseColor[1] = MetalColor.green() / 255.999;
        NewMat->BaseColor[2] = MetalColor.blue() / 255.999;
        NewMat->Fuzzy = MatMetalFuzzy->value();

        RenderPreview->m_NameToMaterialPtr[NowMatName] = NewMat;
    }
    else if(MatTypeStr == "Glass")
    {
        auto NewMat = std::make_shared<GlGlass>();
        NewMat->Name = NowMatName;
        NewMat->IOR = MatGlassIOR->value();

        RenderPreview->m_NameToMaterialPtr[NowMatName] = NewMat;
    }
    else if(MatTypeStr == "Emit")
    {
        auto NewMat = std::make_shared<GlEmit>();
        NewMat->Name = NowMatName;
        NewMat->BaseColor[0] = EmitColor.red() / 255.999;
        NewMat->BaseColor[1] = EmitColor.green() / 255.999;
        NewMat->BaseColor[2] = EmitColor.blue() / 255.999;
        NewMat->Intensity = MatEmitIntensity->value();

        RenderPreview->m_NameToMaterialPtr[NowMatName] = NewMat;
    }else if(MatTypeStr == "Fog")
    {
        auto NewMat = std::make_shared<GlFog>();
        NewMat->Name = NowMatName;

        RenderPreview->m_NameToMaterialPtr[NowMatName] = NewMat;
    }

    RenderPreview->update();
}

void MainWindow::LoadStyle(const QString &QssFile)
{
    QString qss;
    QFile file(QssFile);
    if (file.open(QFile::ReadOnly))
    {
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line;
            in >> line;
            list << line;
        }
        file.close();
        qss = list.join("\n");
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(paletteColor));
        qApp->setStyleSheet(qss);
    }
}