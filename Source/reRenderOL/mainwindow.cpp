#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_MainWidget = new QWidget(this);

    m_MainLayout = new QHBoxLayout(this);
    m_ALayout = new QVBoxLayout(this);
    m_BLayout = new QVBoxLayout(this);

    m_MainLayout->addLayout(m_ALayout);
    m_MainLayout->addLayout(m_BLayout);
    m_MainLayout->setStretchFactor(m_ALayout,2);
    m_MainLayout->setStretchFactor(m_BLayout,8);
    m_MainWidget->setLayout(m_MainLayout);

    m_AToolsLayout = new QGridLayout(this);

    m_AToolsAboutButton = new QPushButton("About",this);
    m_AToolsImportButton = new QPushButton("Import",this);
    m_AToolsDeleteButton = new QPushButton("Delete",this);
    m_AToolsAddButton = new QPushButton("Add",this);
    m_AToolsSortButton = new QPushButton("Sort",this);

    connect(m_AToolsAboutButton,&QPushButton::pressed,this,&MainWindow::showAbout);

    m_AToolsImportMenu = new QMenu(this);
    m_AToolsImportMenuInput = new WA_Input(m_AToolsImportMenu);
    m_AToolsImportMenuSelect = new WA_Button("Select", m_AToolsImportMenu);
    m_AToolsImportMenu->addAction(m_AToolsImportMenuInput);
    m_AToolsImportMenu->addSeparator();
    m_AToolsImportMenu->addAction(m_AToolsImportMenuSelect);
    m_AToolsImportButton->setMenu(m_AToolsImportMenu);
    m_AToolsImportButton->setStyleSheet("QPushButton::menu-indicator{image:none}");
    m_AToolsImportMenuSelect->m_Button->setEnabled(false);
    connect(m_AToolsImportMenuInput->m_Line,&QLineEdit::textChanged,this,&MainWindow::importIDChange);
    connect(m_AToolsImportMenuSelect->m_Button,&QPushButton::pressed,this,&MainWindow::pushImportMesh);

    connect(m_AToolsDeleteButton,&QPushButton::pressed,this,&MainWindow::pushDelete);

    m_AToolsAddMenu = new QMenu(this);
    m_AToolsAddMenuInput = new WA_Input(m_AToolsAddMenu);
    m_AToolsAddMenuCamera = new WA_Button("Camera", m_AToolsAddMenu);
    m_AToolsAddMenuCube = new WA_Button("Stranded Cube", m_AToolsAddMenu,10,5);
    m_AToolsAddMenuSphere = new WA_Button("Prefect Sphere", m_AToolsAddMenu,5,10);
    m_AToolsAddMenu->addAction(m_AToolsAddMenuInput);
    m_AToolsAddMenu->addSeparator();
    m_AToolsAddMenu->addAction(m_AToolsAddMenuCamera);
    m_AToolsAddMenu->addSeparator();
    m_AToolsAddMenu->addAction(m_AToolsAddMenuCube);
    m_AToolsAddMenu->addAction(m_AToolsAddMenuSphere);
    m_AToolsAddButton->setMenu(m_AToolsAddMenu);
    m_AToolsAddButton->setStyleSheet("QPushButton::menu-indicator{image:none}");
    m_AToolsAddMenuCamera->m_Button->setEnabled(false);
    m_AToolsAddMenuCube->m_Button->setEnabled(false);
    m_AToolsAddMenuSphere->m_Button->setEnabled(false);
    connect(m_AToolsAddMenuInput->m_Line,&QLineEdit::textChanged,this,&MainWindow::addIDChange);
    connect(m_AToolsAddMenuCamera->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddCamera);
    connect(m_AToolsAddMenuCube->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddCube);
    connect(m_AToolsAddMenuSphere->m_Button,&QPushButton::pressed,this,&MainWindow::pushAddSphere);

    m_AToolsLayout->addWidget(m_AToolsAboutButton,0,0,1,2);
    m_AToolsLayout->addWidget(m_AToolsImportButton,1,0,1,1);
    m_AToolsLayout->addWidget(m_AToolsDeleteButton,1,1,1,1);
    m_AToolsLayout->addWidget(m_AToolsAddButton,2,0,1,1);
    m_AToolsLayout->addWidget(m_AToolsSortButton,2,1,1,1);

    m_AList = new QListWidget(this);
    m_AList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_AList,&QListWidget::currentItemChanged,this,&MainWindow::changeListItem);

    m_ALayout->addLayout(m_AToolsLayout);
    m_ALayout->addWidget(m_AList);
    m_ALayout->setStretchFactor(m_AToolsLayout,3);
    m_ALayout->setStretchFactor(m_AList,7);

    m_BBTabWidget = new QTabWidget(this);
    m_BBTabSkybox = new QWidget(m_BBTabWidget);
    m_BBTabMaterial = new QWidget(m_BBTabWidget);
    m_BBTabRender = new QWidget(m_BBTabWidget);
    m_BBTabWidget->addTab(m_BBTabSkybox,"Skybox");
    m_BBTabWidget->addTab(m_BBTabMaterial,"Material");
    m_BBTabWidget->addTab(m_BBTabRender,"Render");

    QImage blankImage;
    blankImage.load(":/img/SkyboxDefault_128.png");

    QString Hips[6]=
    {
        "[F]ront","[B]ack","[T]op","[D]own","[L]eft","[R]ight"
    };

    for(int i=0;i<6;++i) m_BBTabSkyboxImages[i] = new QLabel(m_BBTabSkybox);
    for(int i=0;i<6;++i) m_BBTabSkyboxSelect[i] = new QPushButton("Select",m_BBTabSkybox);

    connect(m_BBTabSkyboxSelect[0],&QPushButton::pressed,this,&MainWindow::selectSkyboxF);
    connect(m_BBTabSkyboxSelect[1],&QPushButton::pressed,this,&MainWindow::selectSkyboxB);
    connect(m_BBTabSkyboxSelect[2],&QPushButton::pressed,this,&MainWindow::selectSkyboxT);
    connect(m_BBTabSkyboxSelect[3],&QPushButton::pressed,this,&MainWindow::selectSkyboxD);
    connect(m_BBTabSkyboxSelect[4],&QPushButton::pressed,this,&MainWindow::selectSkyboxL);
    connect(m_BBTabSkyboxSelect[5],&QPushButton::pressed,this,&MainWindow::selectSkyboxR);

    auto TabSkyboxLayout = new QHBoxLayout(this);
    for(int i=0;i<6;++i)
    {
        m_BBTabSkyboxImages[i]->setFixedSize(128,128);
        m_BBTabSkyboxImages[i]->setPixmap(QPixmap::fromImage(blankImage));

        auto thisLayout = new QVBoxLayout(this);
        thisLayout->addWidget(new QLabel(Hips[i]+":",m_BBTabSkybox));
        thisLayout->addWidget(m_BBTabSkyboxImages[i]);
        thisLayout->addWidget(m_BBTabSkyboxSelect[i]);
        thisLayout->addStretch();

        TabSkyboxLayout->addLayout(thisLayout);
    }
    TabSkyboxLayout->addStretch();
    m_BBTabSkybox->setLayout(TabSkyboxLayout);

    m_BALayout = new QHBoxLayout(this);

    m_BAProperty = new QGroupBox("Property",this);
    m_BAPropertyLayout = new QVBoxLayout(m_BAProperty);
    m_BAProperty->setLayout(m_BAPropertyLayout);

    m_BAPropertyLayout->addWidget(new QLabel("Selected ID:",m_BAProperty));

    m_BAPropertySelectedID = new QLineEdit("Nothing",m_BAProperty);
    m_BAPropertySelectedID->setReadOnly(true);
    m_BAPropertyLayout->addWidget(m_BAPropertySelectedID);

    m_BAPropertyLayout->addWidget(new QLabel("Selected Type:",m_BAProperty));

    m_BAPropertySelectedType = new QLineEdit("Nothing",m_BAProperty);
    m_BAPropertySelectedType->setReadOnly(true);
    m_BAPropertyLayout->addWidget(m_BAPropertySelectedType);

    auto HLine = new QFrame(this);
    HLine->setFrameShape(QFrame::HLine);
    HLine->setFrameShadow(QFrame::Plain);

    m_BAPropertyLayout->addWidget(HLine);

    m_BAPropertyLayout->addWidget(new QLabel("Location:",m_BAProperty));
    for(int i=0;i<3;++i) m_BAPropertyLocation[i] = new QDoubleSpinBox(m_BAProperty);
    auto LocationLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) LocationLayout->addWidget(m_BAPropertyLocation[i]);
    m_BAPropertyLayout->addLayout(LocationLayout);

    m_BAPropertyLayout->addWidget(new QLabel("Rotation:",m_BAProperty));
    for(int i=0;i<3;++i) m_BAPropertyRotation[i] = new QDoubleSpinBox(m_BAProperty);
    auto RotationLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) RotationLayout->addWidget(m_BAPropertyRotation[i]);
    m_BAPropertyLayout->addLayout(RotationLayout);

    m_BAPropertyLayout->addWidget(new QLabel("Scale:",m_BAProperty));
    for(int i=0;i<3;++i) m_BAPropertyScale[i] = new QDoubleSpinBox(m_BAProperty);
    auto ScaleLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) ScaleLayout->addWidget(m_BAPropertyScale[i]);
    m_BAPropertyLayout->addLayout(ScaleLayout);

    m_BAPropertyLayout->addWidget(new QLabel("Target:",m_BAProperty));
    for(int i=0;i<3;++i) m_BAPropertyTarget[i] = new QDoubleSpinBox(m_BAProperty);
    auto TargetLayout = new QHBoxLayout(this);
    for(int i=0;i<3;++i) TargetLayout->addWidget(m_BAPropertyTarget[i]);
    m_BAPropertyLayout->addLayout(TargetLayout);

    m_BAPropertyLayout->addStretch();

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

    m_BALayout->addWidget(m_BAProperty);
    m_BALayout->addWidget(m_BAScenes);
    m_BALayout->setStretchFactor(m_BAProperty,2);
    m_BALayout->setStretchFactor(m_BAScenes,6);

    m_BLayout->addLayout(m_BALayout);
    m_BLayout->addWidget(m_BBTabWidget);
    m_BLayout->setStretchFactor(m_BALayout,7);
    m_BLayout->setStretchFactor(m_BBTabWidget,3);

    this->setFixedSize(1480,920);
    this->setCentralWidget(m_MainWidget);

    this->loadStyle(":/qss/psblack.css");
    this->setWindowIcon(QPixmap(":/img/Logo.png"));
}

MainWindow::~MainWindow(){}

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

void MainWindow::changeListItem(QListWidgetItem* Current)
{
    if(Current) m_BAPropertySelectedID->setText(Current->text());
    else m_BAPropertySelectedID->setText("Nothing");
}

void MainWindow::importIDChange(const QString& newText)
{
    if(newText.length() && !m_IDSet.count(newText))
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
    if(ID.length() && !m_IDSet.count(ID))
    {
        QString fileName = QFileDialog::getOpenFileName();
        if(fileName.length())
        {
            m_AList->addItem(ID);
            m_IDSet.insert(ID);
        }

        m_AToolsImportMenuInput->m_Line->setText("");
    }
}

void MainWindow::addIDChange(const QString& newText)
{
    if(newText.length() && !m_IDSet.count(newText))
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
    if(ID.length() && !m_IDSet.count(ID))
    {
        m_AList->addItem(ID);
        m_IDSet.insert(ID);

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushAddCube()
{
    QString ID = m_AToolsAddMenuInput->m_Line->text();
    if(ID.length() && !m_IDSet.count(ID))
    {
        m_AList->addItem(ID);
        m_IDSet.insert(ID);

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushAddSphere()
{
    QString ID = m_AToolsAddMenuInput->m_Line->text();
    if(ID.length() && !m_IDSet.count(ID))
    {
        m_AList->addItem(ID);
        m_IDSet.insert(ID);

        m_AToolsAddMenuInput->m_Line->setText("");
    }
}

void MainWindow::pushDelete()
{
    if(m_AList->currentItem())
    {
        if(m_IDSet.count(m_AList->currentItem()->text()))
        {
            m_IDSet.erase(m_AList->currentItem()->text());
        }
        m_AList->takeItem(m_AList->currentRow());
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
    }
}
