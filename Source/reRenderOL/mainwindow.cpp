#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
    auto MainWidget = new QWidget(this);

    auto MainLayout = DL(new QHBoxLayout());
    auto ALayout = DL(new QVBoxLayout());
    auto BLayout = DL(new QVBoxLayout());

    MainLayout->addLayout(ALayout);
    MainLayout->addLayout(BLayout);
    MainLayout->setStretchFactor(ALayout,2);
    MainLayout->setStretchFactor(BLayout,8);
    MainWidget->setLayout(MainLayout);

    auto AToolsLayout = DL(new QGridLayout());

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

    auto TabSkyboxLayout = DL(new QHBoxLayout());
    for(int i=0;i<6;++i)
    {
        m_BBTabSkyboxImages[i]->setFixedSize(128,128);
        m_BBTabSkyboxImages[i]->setPixmap(QPixmap::fromImage(DefaultImage));

        auto thisLayout = DL(new QVBoxLayout());
        thisLayout->addWidget(new QLabel(Hips[i]+":",BBTabSkybox));
        thisLayout->addWidget(m_BBTabSkyboxImages[i]);
        thisLayout->addWidget(BBTabSkyboxSelect[i]);
        thisLayout->addStretch();

        TabSkyboxLayout->addLayout(thisLayout);
    }
    TabSkyboxLayout->addStretch();
    BBTabSkybox->setLayout(TabSkyboxLayout);
    // Tab: Skybox is End

    m_BBTabMaterialSelectType = new QComboBox(BBTabMaterial);
    auto BBTabMaterialLayout = DL(new QVBoxLayout());
    BBTabMaterial->setLayout(BBTabMaterialLayout);
    BBTabMaterialLayout->addWidget(m_BBTabMaterialSelectType);

    // Material Types
    m_BBTabMaterialSelectType->addItem("Diffuse");
    m_BBTabMaterialSelectType->addItem("Metal");
    m_BBTabMaterialSelectType->addItem("Glass");
    m_BBTabMaterialSelectType->addItem("Emit");

    auto BBTabMaterialParameterLayout = DL(new QHBoxLayout());
    auto BBTabMaterialParameterDetailLayout = DL(new QVBoxLayout());
    BBTabMaterialParameterLayout->addLayout(BBTabMaterialParameterDetailLayout);
    BBTabMaterialParameterDetailLayout->addWidget(new QLabel("Color:",BBTabMaterial));
    BBTabMaterialParameterDetailLayout->addWidget(new QLabel("Light:",BBTabMaterial));
    BBTabMaterialParameterDetailLayout->addStretch();

    BBTabMaterialParameterLayout->addSpacing(128);

    // Add a Line Only for Once
    auto VLine = new QFrame(this);
    VLine->setFrameShape(QFrame::VLine);
    VLine->setFrameShadow(QFrame::Plain);
    BBTabMaterialParameterLayout->addWidget(VLine);

    // Diffuse Texture in Material Tab
    auto BBTabMaterialParameterDiffuseTextureLayout = DL(new QVBoxLayout());
    m_BBTabMaterialDiffuseTextureSelect = new QPushButton("Select",BBTabMaterial);
    m_BBTabMaterialDiffuseTextureImage = new QLabel(BBTabMaterial);
    m_BBTabMaterialDiffuseTextureImage->setFixedSize(128,128);
    m_BBTabMaterialDiffuseTextureImage->setPixmap(QPixmap::fromImage(DefaultImage));
    BBTabMaterialParameterDiffuseTextureLayout->addWidget(new QLabel("Diffuse:"));
    BBTabMaterialParameterDiffuseTextureLayout->addWidget(m_BBTabMaterialDiffuseTextureImage);
    BBTabMaterialParameterDiffuseTextureLayout->addWidget(m_BBTabMaterialDiffuseTextureSelect);
    BBTabMaterialParameterDiffuseTextureLayout->addStretch();
    BBTabMaterialParameterLayout->addLayout(BBTabMaterialParameterDiffuseTextureLayout);

    // Normal Texture in Material Tab
    auto BBTabMaterialParameterNormalTextureLayout = DL(new QVBoxLayout());
    m_BBTabMaterialNormalTextureSelect = new QPushButton("Select",BBTabMaterial);
    m_BBTabMaterialNormalTextureImage = new QLabel(BBTabMaterial);
    m_BBTabMaterialNormalTextureImage->setFixedSize(128,128);
    m_BBTabMaterialNormalTextureImage->setPixmap(QPixmap::fromImage(DefaultImage));
    BBTabMaterialParameterNormalTextureLayout->addWidget(new QLabel("Normal:"));
    BBTabMaterialParameterNormalTextureLayout->addWidget(m_BBTabMaterialNormalTextureImage);
    BBTabMaterialParameterNormalTextureLayout->addWidget(m_BBTabMaterialNormalTextureSelect);
    BBTabMaterialParameterNormalTextureLayout->addStretch();
    BBTabMaterialParameterLayout->addLayout(BBTabMaterialParameterNormalTextureLayout);

    BBTabMaterialParameterLayout->addStretch();
    BBTabMaterialLayout->addLayout(BBTabMaterialParameterLayout);
    BBTabMaterialLayout->addStretch();
    // Material Tab is End

    // BA Layout: Property and Scenes
    auto BALayout = DL(new QHBoxLayout());

    // Property
    auto BAProperty = new QGroupBox("Property",MainWidget);
    auto BAPropertyLayout = DL(new QVBoxLayout());
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
    BAPropertyLayout->addWidget(HLine);











    // Works here

    BAPropertyLayout->addWidget(new QLabel("Location:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyLocation[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyLocation[i]->setMinimum(-99);
        m_BAPropertyLocation[i]->setMaximum(+99);
    }
    auto LocationLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) LocationLayout->addWidget(m_BAPropertyLocation[i]);
    BAPropertyLayout->addLayout(LocationLayout);

    BAPropertyLayout->addWidget(new QLabel("Rotation:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyRotation[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyRotation[i]->setMinimum(-360);
        m_BAPropertyRotation[i]->setMaximum(+360);
    }
    auto RotationLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) RotationLayout->addWidget(m_BAPropertyRotation[i]);
    BAPropertyLayout->addLayout(RotationLayout);

    BAPropertyLayout->addWidget(new QLabel("Scale:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyScale[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyScale[i]->setMinimum(0.1);
        m_BAPropertyScale[i]->setMaximum(100);
    }
    auto ScaleLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) ScaleLayout->addWidget(m_BAPropertyScale[i]);
    BAPropertyLayout->addLayout(ScaleLayout);

    BAPropertyLayout->addWidget(new QLabel("Target:",BAProperty));
    for(int i=0;i<3;++i)
    {
        m_BAPropertyTarget[i] = new QDoubleSpinBox(BAProperty);
        m_BAPropertyTarget[i]->setMinimum(-99);
        m_BAPropertyTarget[i]->setMaximum(+99);
    }
    auto TargetLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) TargetLayout->addWidget(m_BAPropertyTarget[i]);
    BAPropertyLayout->addLayout(TargetLayout);

    BAPropertyLayout->addStretch();

    m_BAScenes = new QTabWidget(this);
    m_BAScenesEdit = new QWidget(m_BAScenes);
    m_BAScenesCamera = new QWidget(m_BAScenes);
    m_BAScenesRender = new QWidget(m_BAScenes);
    m_BAScenes->addTab(m_BAScenesEdit,"Edit");
    m_BAScenes->addTab(m_BAScenesCamera,"Camera");
    m_BAScenes->addTab(m_BAScenesRender,"Render");

    m_BAScenesEditLayout = new QVBoxLayout(this);
    m_BAScenesEditView = new GL_ViewWidget(this);
    m_BAScenesEdit->setLayout(m_BAScenesEditLayout);
    m_BAScenesEditLayout->addWidget(m_BAScenesEditView);

    m_BAScenesCameraLayout = new QVBoxLayout(this);
    m_BAScenesCameraView = new GL_ViewWidget(this);
    m_BAScenesCamera->setLayout(m_BAScenesCameraLayout);
    m_BAScenesCameraLayout->addWidget(m_BAScenesCameraView);

    m_BAScenesRenderLayout = new QVBoxLayout(this);
    m_BAScenesRenderView = new QLabel("Render Result",this);
    m_BAScenesRenderView->setAlignment(Qt::AlignCenter);
    m_BAScenesRender->setLayout(m_BAScenesRenderLayout);
    m_BAScenesRenderLayout->addWidget(m_BAScenesRenderView);

    BALayout->addWidget(BAProperty);
    BALayout->addWidget(m_BAScenes);
    BALayout->setStretchFactor(BAProperty,2);
    BALayout->setStretchFactor(m_BAScenes,6);

    BLayout->addLayout(BALayout);
    BLayout->addWidget(BBTabWidget);
    BLayout->setStretchFactor(BALayout,7);
    BLayout->setStretchFactor(BBTabWidget,3);

    this->setFixedSize(1480,920);
    this->setCentralWidget(MainWidget);

    this->loadStyle(":/qss/psblack.css");
    this->setWindowIcon(QPixmap(":/img/Logo.png"));
}

MainWindow::~MainWindow()
{
    DL_Clear();
}

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
        m_BAPropertySelectedID->setText(Current->text());

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
        }
        break;
        case ECore::OType::SPHERE:
        {
            SelectedType = "Sphere";
            EnableSpinBox(true,m_BAPropertyLocation);
            EnableSpinBox(true,m_BAPropertyRotation);
            EnableSpinBox(true,m_BAPropertyScale);
            EnableSpinBox(false,m_BAPropertyTarget);
        }
        break;
        case ECore::OType::CAMERA:
        {
            SelectedType = "Camera";
            EnableSpinBox(true,m_BAPropertyLocation);
            EnableSpinBox(false,m_BAPropertyRotation);
            EnableSpinBox(false,m_BAPropertyScale);
            EnableSpinBox(true,m_BAPropertyTarget);
        }
        break;
        }

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

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushDelete()
{
    if(m_AList->currentItem())
    {
        if(ECore::Objects.count(m_AList->currentItem()->text()))
        {
            ECore::Objects.erase(m_AList->currentItem()->text());
        }
        m_AList->takeItem(m_AList->currentRow());
    }
}

void MainWindow::pushSort()
{
    m_AList->clear();
    QVector<QString> IDs;
    for(auto i=ECore::Objects.begin();i!=ECore::Objects.end();++i)
    {
        IDs.push_back(i->first);
    }
    IDs.sort();
    for(auto i=IDs.begin();i!=IDs.end();++i)
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
        ECore::Skyboxs[ToIndex[Which]] = selectedImage;
    }
}
