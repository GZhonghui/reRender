#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
    auto MainWidget = new QWidget(this);

    auto MainLayout = new QHBoxLayout();
    auto ALayout = new QVBoxLayout();
    auto BLayout = new QVBoxLayout();

    MainLayout->addLayout(ALayout);
    MainLayout->addLayout(BLayout);
    MainLayout->setStretchFactor(ALayout,2);
    MainLayout->setStretchFactor(BLayout,8);
    MainWidget->setLayout(MainLayout);

    auto AToolsLayout = new QGridLayout();

    auto AToolsAboutButton = new QPushButton("About",MainWidget);
    auto AToolsImportButton = new QPushButton("Import",MainWidget);
    auto AToolsDeleteButton = new QPushButton("Delete",MainWidget);
    auto AToolsAddButton = new QPushButton("Add",MainWidget);
    auto AToolsSortButton = new QPushButton("Sort",MainWidget);

    // Tools: About Delete Sort
    connect(AToolsAboutButton,&QPushButton::pressed,this,&MainWindow::showAbout);
    connect(AToolsDeleteButton,&QPushButton::pressed,this,&MainWindow::pushDelete);
    connect(AToolsSortButton,&QPushButton::pressed,this,&MainWindow::pushSort);

    // Tool: Import
    auto AToolsImportMenu = new QMenu(AToolsImportButton);
    m_AToolsImportMenuInput = new WA_Input(AToolsImportMenu);
    m_AToolsImportMenuSelect = new WA_Button("Select", AToolsImportMenu);
    AToolsImportMenu->addAction(m_AToolsImportMenuInput);
    AToolsImportMenu->addSeparator();
    AToolsImportMenu->addAction(m_AToolsImportMenuSelect);
    AToolsImportButton->setMenu(AToolsImportMenu);
    AToolsImportButton->setStyleSheet("QPushButton::menu-indicator{image:none}");
    m_AToolsImportMenuSelect->m_Button->setEnabled(false);
    connect(m_AToolsImportMenuInput->m_Line,&QLineEdit::textChanged,this,&MainWindow::importIDChange);
    connect(m_AToolsImportMenuSelect->m_Button,&QPushButton::pressed,this,&MainWindow::pushImportMesh);

    // Tool: Add
    auto AToolsAddMenu = new QMenu(AToolsAddButton);
    m_AToolsAddMenuInput = new WA_Input(AToolsAddMenu);
    m_AToolsAddMenuCamera = new WA_Button("Camera", AToolsAddMenu);
    m_AToolsAddMenuCube = new WA_Button("Stranded Cube", AToolsAddMenu,10,5);
    m_AToolsAddMenuSphere = new WA_Button("Prefect Sphere", AToolsAddMenu,5,10);
    AToolsAddMenu->addAction(m_AToolsAddMenuInput);
    AToolsAddMenu->addSeparator();
    AToolsAddMenu->addAction(m_AToolsAddMenuCamera);
    AToolsAddMenu->addSeparator();
    AToolsAddMenu->addAction(m_AToolsAddMenuCube);
    AToolsAddMenu->addAction(m_AToolsAddMenuSphere);
    AToolsAddButton->setMenu(AToolsAddMenu);
    AToolsAddButton->setStyleSheet("QPushButton::menu-indicator{image:none}");
    m_AToolsAddMenuCamera->m_Button->setEnabled(false);
    m_AToolsAddMenuCube->m_Button->setEnabled(false);
    m_AToolsAddMenuSphere->m_Button->setEnabled(false);
    connect(m_AToolsAddMenuInput->m_Line,&QLineEdit::textChanged,this,&MainWindow::addIDChange);
    connect(m_AToolsAddMenuCamera->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddCamera);
    connect(m_AToolsAddMenuCube->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddCube);
    connect(m_AToolsAddMenuSphere->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddSphere);

    // Tools Grid
    AToolsLayout->addWidget(AToolsAboutButton,0,0,1,2);
    AToolsLayout->addWidget(AToolsImportButton,1,0,1,1);
    AToolsLayout->addWidget(AToolsDeleteButton,1,1,1,1);
    AToolsLayout->addWidget(AToolsAddButton,2,0,1,1);
    AToolsLayout->addWidget(AToolsSortButton,2,1,1,1);

    m_AList = new QListWidget(MainWidget);
    m_AList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_AList,&QListWidget::currentItemChanged,this,&MainWindow::changeListItem);

    ALayout->addLayout(AToolsLayout);
    ALayout->addWidget(m_AList);
    ALayout->setStretchFactor(AToolsLayout,3);
    ALayout->setStretchFactor(m_AList,7);
    // Left is End Here

    // BB Tabs
    auto BBTabWidget = new QTabWidget(MainWidget);
    auto BBTabSkybox = new QWidget(BBTabWidget);
    auto BBTabCamera = new QWidget(BBTabWidget);
    auto BBTabMaterial = new QWidget(BBTabWidget);
    auto BBTabRender = new QWidget(BBTabWidget);
    BBTabWidget->addTab(BBTabSkybox,"Skybox");
    BBTabWidget->addTab(BBTabCamera,"Camera");
    BBTabWidget->addTab(BBTabMaterial,"Material");
    BBTabWidget->addTab(BBTabRender,"Render");

    // A Default Image 128x128
    QImage DefaultImage;
    DefaultImage.load(":/img/SkyboxDefault_128.png");

    // Use in Tab Skybox
    QString Hips[6]=
    {
        "[F]ront","[B]ack","[T]op","[D]own","[L]eft","[R]ight"
    };

    QPushButton* BBTabSkyboxSelect[6];

    for(int i=0;i<6;++i) m_BBTabSkyboxImages[i] = new QLabel(BBTabSkybox);
    for(int i=0;i<6;++i) BBTabSkyboxSelect[i] = new QPushButton("Select",BBTabSkybox);

    connect(BBTabSkyboxSelect[0],&QPushButton::pressed,this,&MainWindow::selectSkyboxF);
    connect(BBTabSkyboxSelect[1],&QPushButton::pressed,this,&MainWindow::selectSkyboxB);
    connect(BBTabSkyboxSelect[2],&QPushButton::pressed,this,&MainWindow::selectSkyboxT);
    connect(BBTabSkyboxSelect[3],&QPushButton::pressed,this,&MainWindow::selectSkyboxD);
    connect(BBTabSkyboxSelect[4],&QPushButton::pressed,this,&MainWindow::selectSkyboxL);
    connect(BBTabSkyboxSelect[5],&QPushButton::pressed,this,&MainWindow::selectSkyboxR);

    auto TabSkyboxLayout = new QHBoxLayout();
    for(int i=0;i<6;++i)
    {
        m_BBTabSkyboxImages[i]->setFixedSize(128,128);
        m_BBTabSkyboxImages[i]->setPixmap(QPixmap::fromImage(DefaultImage));

        auto thisLayout = new QVBoxLayout();
        thisLayout->addWidget(new QLabel(Hips[i]+":",BBTabSkybox));
        thisLayout->addWidget(m_BBTabSkyboxImages[i]);
        thisLayout->addWidget(BBTabSkyboxSelect[i]);
        thisLayout->addStretch();

        TabSkyboxLayout->addLayout(thisLayout);
    }
    TabSkyboxLayout->addStretch();
    BBTabSkybox->setLayout(TabSkyboxLayout);
    // Tab: Skybox is End

    m_BBTabCameraSelectWhich = new QComboBox(BBTabCamera);
    auto BBTabCameraLayout = new QVBoxLayout();
    BBTabCamera->setLayout(BBTabCameraLayout);
    BBTabCameraLayout->addWidget(m_BBTabCameraSelectWhich);

    connect(m_BBTabCameraSelectWhich,&QComboBox::currentIndexChanged,this,&MainWindow::cameraChanged);

    m_BBTabCameraResolutionWidth = new QSpinBox(BBTabCamera);
    m_BBTabCameraResolutionHeight = new QSpinBox(BBTabCamera);
    m_BBTabCameraResolutionWidth->setMinimum(128);
    m_BBTabCameraResolutionWidth->setMaximum(2048);
    m_BBTabCameraResolutionWidth->setEnabled(false);
    m_BBTabCameraResolutionHeight->setMinimum(128);
    m_BBTabCameraResolutionHeight->setMaximum(2048);
    m_BBTabCameraResolutionHeight->setEnabled(false);

    auto BBTabCameraResolutionLayout = new QHBoxLayout();
    BBTabCameraResolutionLayout->addWidget(new QLabel("Resolution: ",BBTabCamera));
    BBTabCameraResolutionLayout->addWidget(m_BBTabCameraResolutionWidth);
    BBTabCameraResolutionLayout->addWidget(new QLabel(" x ",BBTabCamera));
    BBTabCameraResolutionLayout->addWidget(m_BBTabCameraResolutionHeight);
    BBTabCameraResolutionLayout->addStretch();
    BBTabCameraLayout->addLayout(BBTabCameraResolutionLayout);

    m_BBTabCameraFOV = new QDoubleSpinBox(BBTabCamera);
    m_BBTabCameraFOV->setMinimum(45.0);
    m_BBTabCameraFOV->setMaximum(120.0);
    m_BBTabCameraFOV->setEnabled(false);
    auto BBTabCameraFOVLayout = new QHBoxLayout();
    BBTabCameraFOVLayout->addWidget(new QLabel("FOV: ",BBTabCamera));
    BBTabCameraFOVLayout->addWidget(m_BBTabCameraFOV);
    BBTabCameraFOVLayout->addStretch();
    BBTabCameraLayout->addLayout(BBTabCameraFOVLayout);
    
    BBTabCameraLayout->addStretch();

    m_BBTabCameraSelectWhich->setEnabled(false);

    m_BBTabMaterialSelectType = new QComboBox(BBTabMaterial);
    auto BBTabMaterialLayout = new QVBoxLayout();
    BBTabMaterial->setLayout(BBTabMaterialLayout);
    BBTabMaterialLayout->addWidget(m_BBTabMaterialSelectType);

    // Material Types
    m_BBTabMaterialSelectType->addItem("Diffuse");
    m_BBTabMaterialSelectType->addItem("Metal");
    m_BBTabMaterialSelectType->addItem("Glass");
    m_BBTabMaterialSelectType->addItem("Emit");

    m_BBTabMaterialSelectType->setEnabled(false);

    connect(m_BBTabMaterialSelectType,&QComboBox::currentIndexChanged,this,&MainWindow::materialTypeChanged);

    m_BBTabMaterialStacked = new QStackedWidget(BBTabMaterial);
    m_BBTabMaterialStacked->setEnabled(false);

    BBTabMaterialLayout->addWidget(m_BBTabMaterialStacked);
    BBTabMaterialLayout->addStretch();

    auto BBTabMaterialStackedDiffuse = new QWidget(m_BBTabMaterialStacked);
    auto BBTabMaterialStackedMetal = new QWidget(m_BBTabMaterialStacked);
    auto BBTabMaterialStackedGlass = new QWidget(m_BBTabMaterialStacked);
    auto BBTabMaterialStackedEmit = new QWidget(m_BBTabMaterialStacked);

    m_BBTabMaterialStacked->addWidget(BBTabMaterialStackedDiffuse);
    m_BBTabMaterialStacked->addWidget(BBTabMaterialStackedMetal);
    m_BBTabMaterialStacked->addWidget(BBTabMaterialStackedGlass);
    m_BBTabMaterialStacked->addWidget(BBTabMaterialStackedEmit);

    // Diffuse Page
    auto BBTabMaterialStackedDiffuseLayout = new QHBoxLayout();
    BBTabMaterialStackedDiffuse->setLayout(BBTabMaterialStackedDiffuseLayout);

    auto BBTabMaterialStackedDiffuseLeftLayout = new QVBoxLayout();
    BBTabMaterialStackedDiffuseLeftLayout->addWidget(new QLabel("Diffuse Color:",BBTabMaterialStackedDiffuse));
    m_BBTabMaterialStackedDiffuseBaseColor = new Color_Button(BBTabMaterialStackedDiffuse);
    BBTabMaterialStackedDiffuseLeftLayout->addWidget(m_BBTabMaterialStackedDiffuseBaseColor);
    BBTabMaterialStackedDiffuseLeftLayout->addStretch();

    BBTabMaterialStackedDiffuseLayout->addLayout(BBTabMaterialStackedDiffuseLeftLayout);
    BBTabMaterialStackedDiffuseLayout->addStretch();

    // Metal Page
    auto BBTabMaterialStackedMetalLayout = new QHBoxLayout();
    BBTabMaterialStackedMetal->setLayout(BBTabMaterialStackedMetalLayout);

    auto BBTabMaterialStackedMetalLeftLayout = new QVBoxLayout();
    BBTabMaterialStackedMetalLeftLayout->addWidget(new QLabel("Metal Color:",BBTabMaterialStackedMetal));
    m_BBTabMaterialStackedMetalBaseColor = new Color_Button(BBTabMaterialStackedMetal);
    BBTabMaterialStackedMetalLeftLayout->addWidget(m_BBTabMaterialStackedMetalBaseColor);
    BBTabMaterialStackedMetalLeftLayout->addStretch();

    BBTabMaterialStackedMetalLayout->addLayout(BBTabMaterialStackedMetalLeftLayout);
    BBTabMaterialStackedMetalLayout->addStretch();
    
    // Glass Page

    // Emit Page
    auto BBTabMaterialStackedEmitLayout = new QHBoxLayout();
    BBTabMaterialStackedEmit->setLayout(BBTabMaterialStackedEmitLayout);

    auto BBTabMaterialStackedEmitLeftLayout = new QVBoxLayout();
    BBTabMaterialStackedEmitLeftLayout->addWidget(new QLabel("Emit Color:",BBTabMaterialStackedEmit));
    m_BBTabMaterialStackedEmitBaseColor = new Color_Button(BBTabMaterialStackedEmit);
    BBTabMaterialStackedEmitLeftLayout->addWidget(m_BBTabMaterialStackedEmitBaseColor);
    BBTabMaterialStackedEmitLeftLayout->addStretch();

    BBTabMaterialStackedEmitLayout->addLayout(BBTabMaterialStackedEmitLeftLayout);
    BBTabMaterialStackedEmitLayout->addStretch();

    // Material Tab is End

    auto BBTabRenderRenderButton = new QPushButton("Render",BBTabRender);
    connect(BBTabRenderRenderButton,&QPushButton::pressed,this,&MainWindow::pushRender);

    auto BBTabRenderLayout = new QVBoxLayout();
    BBTabRenderLayout->addWidget(BBTabRenderRenderButton);

    m_RenderProgress = new QProgressBar(BBTabRender);
    BBTabRenderLayout->addWidget(m_RenderProgress);
    m_RenderProgress->setRange(0,100);
    m_RenderProgress->setValue(0);

    BBTabRenderLayout->addStretch();
    BBTabRender->setLayout(BBTabRenderLayout);
    // Render Tab is End

    // BA Layout: Property and Scenes
    auto BALayout = new QHBoxLayout();

    // Property
    auto BAProperty = new QGroupBox("Property",MainWidget);
    auto BAPropertyLayout = new QVBoxLayout();
    BAProperty->setLayout(BAPropertyLayout);

    BAPropertyLayout->addWidget(new QLabel("Selected ID:",BAProperty));
    m_BAPropertySelectedID = new QLineEdit("Nothing",BAProperty);
    m_BAPropertySelectedID->setReadOnly(true);
    BAPropertyLayout->addWidget(m_BAPropertySelectedID);

    BAPropertyLayout->addWidget(new QLabel("Selected Type:",BAProperty));
    m_BAPropertySelectedType = new QLineEdit("Nothing",BAProperty);
    m_BAPropertySelectedType->setReadOnly(true);
    BAPropertyLayout->addWidget(m_BAPropertySelectedType);

    // Add a Line Only for Once
    auto HLine = new QFrame(this);
    HLine->setFrameShape(QFrame::HLine);
    HLine->setFrameShadow(QFrame::Plain);
    BAPropertyLayout->addSpacing(64);
    BAPropertyLayout->addWidget(HLine);

    // Input Location
    BAPropertyLayout->addWidget(new QLabel("Location:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyLocation[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyLocation[i]->setMinimum(-99);
        m_BAPropertyLocation[i]->setMaximum(+99);
        m_BAPropertyLocation[i]->setEnabled(false);
    }
    auto LocationLayout = new QHBoxLayout();
    for(int i=0;i<3;++i) LocationLayout->addWidget(m_BAPropertyLocation[i]);
    BAPropertyLayout->addLayout(LocationLayout);

    // Input Rotation
    BAPropertyLayout->addWidget(new QLabel("Rotation:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyRotation[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyRotation[i]->setMinimum(-360);
        m_BAPropertyRotation[i]->setMaximum(+360);
        m_BAPropertyRotation[i]->setEnabled(false);
    }
    auto RotationLayout = new QHBoxLayout();
    for(int i=0;i<3;++i) RotationLayout->addWidget(m_BAPropertyRotation[i]);
    BAPropertyLayout->addLayout(RotationLayout);

    // Input Scale
    BAPropertyLayout->addWidget(new QLabel("Scale:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyScale[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyScale[i]->setMinimum(0.1);
        m_BAPropertyScale[i]->setMaximum(100);
        m_BAPropertyScale[i]->setEnabled(false);
    }
    auto ScaleLayout = new QHBoxLayout();
    for(int i=0;i<3;++i) ScaleLayout->addWidget(m_BAPropertyScale[i]);
    BAPropertyLayout->addLayout(ScaleLayout);

    // Input Target
    BAPropertyLayout->addWidget(new QLabel("Target:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyTarget[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyTarget[i]->setMinimum(-99);
        m_BAPropertyTarget[i]->setMaximum(+99);
        m_BAPropertyTarget[i]->setEnabled(false);
    }
    auto TargetLayout = new QHBoxLayout();
    for(int i=0;i<3;++i) TargetLayout->addWidget(m_BAPropertyTarget[i]);
    BAPropertyLayout->addLayout(TargetLayout);

    BAPropertyLayout->addStretch();
    // Property is End

    auto BAScenes = new QTabWidget(MainWidget);
    auto BAScenesEdit = new QWidget(BAScenes);
    auto BAScenesCamera = new QWidget(BAScenes);
    auto BAScenesRender = new QWidget(BAScenes);
    BAScenes->addTab(BAScenesEdit,"Edit");
    BAScenes->addTab(BAScenesCamera,"Camera");
    BAScenes->addTab(BAScenesRender,"Render");

    auto BAScenesEditLayout = new QVBoxLayout();
    m_BAScenesEditView = new GL_ViewWidget(BAScenes);
    BAScenesEdit->setLayout(BAScenesEditLayout);
    BAScenesEditLayout->addWidget(m_BAScenesEditView);

    auto BAScenesCameraLayout = new QVBoxLayout();
    m_BAScenesCameraView = new QLabel("Camera Live",BAScenes);
    m_BAScenesCameraView->setAlignment(Qt::AlignCenter);
    BAScenesCamera->setLayout(BAScenesCameraLayout);
    BAScenesCameraLayout->addWidget(m_BAScenesCameraView);

    auto BAScenesRenderLayout = new QVBoxLayout();
    m_BAScenesRenderView = new QLabel("Render Result",BAScenes);
    m_BAScenesRenderView->setAlignment(Qt::AlignCenter);
    BAScenesRender->setLayout(BAScenesRenderLayout);
    BAScenesRenderLayout->addWidget(m_BAScenesRenderView);
    // 3 Scenes are End

    BALayout->addWidget(BAProperty);
    BALayout->addWidget(BAScenes);
    BALayout->setStretchFactor(BAProperty,2);
    BALayout->setStretchFactor(BAScenes,6);

    BLayout->addLayout(BALayout);
    BLayout->addWidget(BBTabWidget);
    BLayout->setStretchFactor(BALayout,7);
    BLayout->setStretchFactor(BBTabWidget,3);

    this->setFixedSize(1480,920);
    this->setCentralWidget(MainWidget);

    this->loadStyle(":/qss/psblack.css");
    this->setWindowIcon(QPixmap(":/img/Logo.png"));
}

MainWindow::~MainWindow() {}

void MainWindow::loadStyle(const QString &qssFile)
{
    QString qss;
    QFile file(qssFile);
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

void MainWindow::showAbout()
{
    QDialog aboutDialog(this);
    aboutDialog.setFixedSize(680,480);
    QVBoxLayout aboutLayout;

    QLabel LogoLabel;
    QPixmap Logo(":/img/reRender.png");
    LogoLabel.setPixmap(Logo);
    LogoLabel.setAlignment(Qt::AlignCenter);
    aboutLayout.addWidget(&LogoLabel);

    aboutLayout.addStretch();

    QFont fontTitle;
    fontTitle.setFamily("MV Boli");
    fontTitle.setPixelSize(24);

    QFont fontText;
    fontText.setFamily("Times New Roman");
    fontText.setPixelSize(16);

    QLabel aboutTitle;
    aboutTitle.setFont(fontTitle);
    aboutTitle.setText("This is reRender: A Open Source Render Engine");
    aboutTitle.setAlignment(Qt::AlignCenter);
    aboutLayout.addWidget(&aboutTitle);

    QString aboutTextContent;
    aboutTextContent+="\n";
    aboutTextContent+="reRender v0.1 is a C++ Program. reRender v1.0 is a Python Module.\n";
    aboutTextContent+="And this is reRender v2.0, a Standalone Software with UI.\n";
    aboutTextContent+="This Engine is my undergraduate Graduation Design.\n";
    aboutTextContent+="I'm GZhonghui. I got my Bachelor's Degree from CQUPT, China, in 2022.\n";
    aboutTextContent+="My major is Digital Media Technology.\n";
    aboutTextContent+="Mail: mail@gzher.com. Github: github.com/GZhonghui.\n";

    QLabel aboutText;
    aboutText.setText(aboutTextContent);
    aboutText.setAlignment(Qt::AlignCenter);
    aboutText.setFont(fontText);
    aboutLayout.addWidget(&aboutText);

    aboutDialog.setLayout(&aboutLayout);
    aboutDialog.exec();
}

// Important
void MainWindow::changeListItem(QListWidgetItem* Current)
{
    auto EnableSpinBox = [](bool Y, QDoubleSpinBox** P, int count = 3)
    {
        for(int i=0;i<count;++i)
        {
            P[i]->setEnabled(Y);
            P[i]->setValue(0);
        }
    };
    if(Current)
    {
        int ThisMaterialType = 0;
        switch(ECore::Objects[Current->text()]->m_MType)
        {
        case ECore::MType::DIFFUSE:
        {
            ThisMaterialType = 0;
        }
        break;
        case ECore::MType::METAL:
        {
            ThisMaterialType = 1;
        }
        break;
        case ECore::MType::GLASS:
        {
            ThisMaterialType = 2;
        }
        break;
        case ECore::MType::EMIT:
        {
            ThisMaterialType = 3;
        }
        break;
        }
        // Index starts at 0
        m_BBTabMaterialSelectType->setCurrentIndex(ThisMaterialType);

        QString SelectedType = "Nothing";
        switch(ECore::Objects[Current->text()]->m_OType)
        {
        case ECore::OType::MESH:
        {
            SelectedType = "Mesh";
            EnableSpinBox(true,m_BAPropertyLocation);
            EnableSpinBox(true,m_BAPropertyRotation);
            EnableSpinBox(true,m_BAPropertyScale);
            EnableSpinBox(false,m_BAPropertyTarget);

            m_BBTabMaterialSelectType->setEnabled(true);
            m_BBTabMaterialStacked->setEnabled(true);
        }
        break;
        case ECore::OType::SPHERE:
        {
            SelectedType = "Sphere";
            EnableSpinBox(true,m_BAPropertyLocation);
            EnableSpinBox(true,m_BAPropertyRotation);
            EnableSpinBox(true,m_BAPropertyScale);
            EnableSpinBox(false,m_BAPropertyTarget);

            m_BBTabMaterialSelectType->setEnabled(true);
            m_BBTabMaterialStacked->setEnabled(true);
        }
        break;
        case ECore::OType::CAMERA:
        {
            SelectedType = "Camera";
            EnableSpinBox(true,m_BAPropertyLocation);
            EnableSpinBox(false,m_BAPropertyRotation);
            EnableSpinBox(false,m_BAPropertyScale);
            EnableSpinBox(true,m_BAPropertyTarget);

            m_BBTabMaterialSelectType->setEnabled(false);
            m_BBTabMaterialStacked->setEnabled(false);
        }
        break;
        }

        m_BAPropertySelectedID->setText(Current->text());
        m_BAPropertySelectedType->setText(SelectedType);
    }
    else
    {
        m_BAPropertySelectedID->setText("Nothing");
        m_BAPropertySelectedType->setText("Nothing");

        EnableSpinBox(false,m_BAPropertyLocation);
        EnableSpinBox(false,m_BAPropertyRotation);
        EnableSpinBox(false,m_BAPropertyScale);
        EnableSpinBox(false,m_BAPropertyTarget);

        m_BBTabMaterialSelectType->setEnabled(false);
        m_BBTabMaterialStacked->setEnabled(false);
    }
}

void MainWindow::importIDChange(const QString& newText)
{
    if(newText.length() && !ECore::Objects.count(newText))
    {
        m_AToolsImportMenuSelect->m_Button->setEnabled(true);
    }else
    {
        m_AToolsImportMenuSelect->m_Button->setEnabled(false);
    }
}

void MainWindow::pushImportMesh()
{
    QString ID = m_AToolsImportMenuInput->m_Line->text();
    if(ID.length() && !ECore::Objects.count(ID))
    {
        QString fileName = QFileDialog::getOpenFileName();
        if(fileName.length())
        {
            m_AList->addItem(ID);
            ECore::Objects[ID] = std::make_unique<ECore::SceneObj>();
            ECore::Objects[ID]->m_ID = ID;
            ECore::Objects[ID]->m_OType = ECore::OType::MESH;
            ECore::Objects[ID]->m_MType = ECore::MType::DIFFUSE;
        }

        m_AToolsImportMenuInput->m_Line->setText("");
    }
}

void MainWindow::addIDChange(const QString& newText)
{
    if(newText.length() && !ECore::Objects.count(newText))
    {
        m_AToolsAddMenuCamera->m_Button->setEnabled(true);
        m_AToolsAddMenuCube->m_Button->setEnabled(true);
        m_AToolsAddMenuSphere->m_Button->setEnabled(true);
    }else
    {
        m_AToolsAddMenuCamera->m_Button->setEnabled(false);
        m_AToolsAddMenuCube->m_Button->setEnabled(false);
        m_AToolsAddMenuSphere->m_Button->setEnabled(false);
    }
}

void MainWindow::pushAddCamera()
{
    QString ID = m_AToolsAddMenuInput->m_Line->text();
    if(ID.length() && !ECore::Objects.count(ID))
    {
        m_AList->addItem(ID);
        ECore::Objects[ID] = std::make_unique<ECore::SceneObj>();
        ECore::Objects[ID]->m_ID = ID;
        ECore::Objects[ID]->m_OType = ECore::OType::CAMERA;
        ECore::Objects[ID]->m_MType = ECore::MType::DIFFUSE;

        m_BBTabCameraSelectWhich->setEnabled(true);
        m_BBTabCameraSelectWhich->addItem(ID);

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushAddCube()
{
    QString ID = m_AToolsAddMenuInput->m_Line->text();
    if(ID.length() && !ECore::Objects.count(ID))
    {
        m_AList->addItem(ID);
        ECore::Objects[ID] = std::make_unique<ECore::SceneObj>();
        ECore::Objects[ID]->m_ID = ID;
        ECore::Objects[ID]->m_OType = ECore::OType::MESH;
        ECore::Objects[ID]->m_MType = ECore::MType::DIFFUSE;

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushAddSphere()
{
    QString ID = m_AToolsAddMenuInput->m_Line->text();
    if(ID.length() && !ECore::Objects.count(ID))
    {
        m_AList->addItem(ID);
        ECore::Objects[ID] = std::make_unique<ECore::SceneObj>();
        ECore::Objects[ID]->m_ID = ID;
        ECore::Objects[ID]->m_OType = ECore::OType::SPHERE;
        ECore::Objects[ID]->m_MType = ECore::MType::DIFFUSE;

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushDelete()
{
    if(m_AList->currentItem())
    {
        QString ID = m_AList->currentItem()->text();
        if(ECore::Objects.count(ID))
        {
            if(ECore::Objects[ID]->m_OType == ECore::OType::CAMERA)
            {
                int CurrentCameraIndex = m_BBTabCameraSelectWhich->findText(ID);
                if(CurrentCameraIndex != -1)
                {
                    m_BBTabCameraSelectWhich->removeItem(CurrentCameraIndex);
                    if(!m_BBTabCameraSelectWhich->count())
                    {
                        m_BBTabCameraSelectWhich->setEnabled(false);
                    }
                }
            }
            ECore::Objects.erase(ID);
        }
        m_AList->takeItem(m_AList->currentRow());
    }
}

void MainWindow::pushSort()
{
    m_AList->clear();
    QVector<QString> IDArray;
    for(auto i=ECore::Objects.begin();i!=ECore::Objects.end();++i)
    {
        IDArray.push_back(i->first);
    }
    IDArray.sort();
    for(auto i=IDArray.begin();i!=IDArray.end();++i)
    {
        m_AList->addItem(*i);
    }
}

void MainWindow::selectSkybox(const QString& filePath, char Which)
{
    std::unordered_map<char,int> ToIndex;
    const char* Index = "FBTDLR";
    for(int i=0;i<6;++i) ToIndex[Index[i]] = i;

    QImage selectedImage;
    if(filePath.length() && selectedImage.load(filePath))
    {
        auto smallImage = selectedImage.scaled(128,128);
        m_BBTabSkyboxImages[ToIndex[Which]]->setPixmap(QPixmap::fromImage(smallImage));
        ECore::Skyboxs[ToIndex[Which]] = selectedImage; // Save the full image

        m_BAScenesEditView->changeSkybox(ToIndex[Which], selectedImage);
    }
}

// Index starts at 0
void MainWindow::cameraChanged(int Index)
{
    if(Index >= 0)
    {
        m_BBTabCameraResolutionWidth->setEnabled(true);
        m_BBTabCameraResolutionHeight->setEnabled(true);
        m_BBTabCameraFOV->setEnabled(true);
    }else
    {
        m_BBTabCameraResolutionWidth->setEnabled(false);
        m_BBTabCameraResolutionHeight->setEnabled(false);
        m_BBTabCameraFOV->setEnabled(false);
    }
}

// Index starts at 0
void MainWindow::materialTypeChanged(int Index)
{
    auto Current = m_AList->currentItem();
    if(Current)
    {
        ECore::MType newType = ECore::MType::DIFFUSE;
        switch(Index)
        {
        case 0:
        {
            newType = ECore::MType::DIFFUSE;
        }
        break;
        case 1:
        {
            newType = ECore::MType::METAL;
        }
        break;
        case 2:
        {
            newType = ECore::MType::GLASS;
        }
        break;
        case 3:
        {
            newType = ECore::MType::EMIT;
        }
        break;
        }
        ECore::Objects[Current->text()]->m_MType = newType;

        m_BBTabMaterialStacked->setCurrentIndex(Index);
    }
}

// Important: Push Render Button
void MainWindow::pushRender()
{
    
}