#include "RenderDialog.h"

RenderDialog::RenderDialog(float* ResultBuffer, int W, int H, PSceneRequest* SceneData) :
    RenderResultBuffer(ResultBuffer), RenderWidth(W), RenderHeight(H), SceneData(SceneData)
{
    setFixedSize(560, 720);

    auto MainLayout = new QGridLayout;
    setLayout(MainLayout);

    auto ResolutionLabel = new QLabel(QString("Resolution = %1 * %2").arg(QString::number(W), QString::number(H)));
    ResolutionLabel->setAlignment(Qt::AlignCenter);
    MainLayout->addWidget(ResolutionLabel, 0, 0, 1, 2);

    RunList = new QListWidget;
    RunList->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    MainLayout->addWidget(RunList, 1, 0, 1, 2);

    auto ButtonItem = new QListWidgetItem;
    auto AddRunTargetButton = new QPushButton("Add  Target");
    QObject::connect(AddRunTargetButton, &QPushButton::clicked, this, &RenderDialog::AddRunTarget);
    ButtonItem->setSizeHint(AddRunTargetButton->sizeHint());
    RunList->addItem(ButtonItem);
    RunList->setItemWidget(ButtonItem, AddRunTargetButton);

    auto RenderButton = new QPushButton("Render");
    QObject::connect(RenderButton, &QPushButton::clicked, this, &RenderDialog::OnPushRender);
    MainLayout->addWidget(RenderButton, 2, 0, 1, 1);

    auto StopButton = new QPushButton("Stop");
    QObject::connect(StopButton, &QPushButton::clicked, this, &RenderDialog::OnPushStop);
    MainLayout->addWidget(StopButton, 2, 1, 1, 1);
    StopButton->setDisabled(true);

    // Default, Render Target Buffer
    int PixelIndex = 0;
    for(int i = 0; i < RenderHeight; i += 1)
    {
        for(int j = 0; j < RenderWidth; j += 1)
        {
            ResultBuffer[PixelIndex + 0] = 0.5f;
            ResultBuffer[PixelIndex + 1] = 1.0 * i / RenderHeight;
            ResultBuffer[PixelIndex + 2] = 1.0 * j / RenderWidth;

            PixelIndex += 3;
        }
    }

    SumSpp = 0;
    SumRenderResult = std::make_unique<float[]>(RenderWidth * RenderHeight * 3);
    for(int i = 0; i < RenderWidth * RenderHeight * 3; i += 1)
    {
        SumRenderResult[i] = 0.0f;
    }
}

void RenderDialog::AddRunTarget()
{
    if(RunList)
    {
        TargetIndex += 1;
        
        auto Item = new QListWidgetItem;
        auto RunTargetWidget = new RunTarget(TargetIndex, RenderWidth, RenderHeight, SceneData);
        QObject::connect(RunTargetWidget, &RunTarget::RemoveSelf, this, &RenderDialog::RemoveRunTargetById);
        QObject::connect(RunTargetWidget, &RunTarget::RenderFinish, this, &RenderDialog::TargetRenderFinish);
        Item->setSizeHint(RunTargetWidget->sizeHint());

        RunList->insertItem(RunList->count() - 1, Item);
        RunList->setItemWidget(Item, RunTargetWidget);
    }
}

void RenderDialog::RemoveRunTargetById(int ID)
{
    if(RunList)
    {
        for(int i = 0; i < RunList->count(); i += 1)
        {
            auto Item = RunList->item(i);
            auto ItemWidget = dynamic_cast<RunTarget*>(RunList->itemWidget(Item));

            if(ItemWidget && ItemWidget->GetID() == ID)
            {
                delete Item;
            }
        }
    }
}

void RenderDialog::TargetRenderFinish(float* Buffer, int Spp)
{
    SumSpp += Spp;
    for(int i = 0; i < RenderWidth * RenderHeight * 3; i += 1)
    {
        SumRenderResult[i] += Buffer[i];

        RenderResultBuffer[i] = std::sqrt(SumRenderResult[i] / SumSpp);
    }
}

void RenderDialog::OnPushRender()
{
    if(RunList)
    {
        for(int i = 0; i < RunList->count(); i += 1)
        {
            auto Item = RunList->item(i);
            auto ItemWidget = dynamic_cast<RunTarget*>(RunList->itemWidget(Item));

            if(ItemWidget && ItemWidget->GetConnected())
            {
                ItemWidget->StartRender();
            }
        }
    }
}

void RenderDialog::OnPushStop()
{
    
}