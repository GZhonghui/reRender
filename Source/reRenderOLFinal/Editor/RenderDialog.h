#ifndef RENDERDIALOG_H
#define RENDERDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QListWidget>

#include <QPushButton>

#include "RunTarget.h"

#include "RenderMessage.pb.h"

class RenderDialog : public QDialog
{
    Q_OBJECT

public:
    RenderDialog(float* ResultBuffer, int W, int H, PSceneRequest* SceneData);

private:
    float* RenderResultBuffer = nullptr;
    int RenderWidth;
    int RenderHeight;

    PSceneRequest* SceneData = nullptr;

    int SumSpp;
    std::unique_ptr<float[]> SumRenderResult;

public:
    void AddRunTarget();
    void RemoveRunTargetById(int ID);
    void TargetRenderFinish(float* Buffer, int Spp);

public:
    void OnPushRender();
    void OnPushStop();

private:
    QListWidget* RunList = nullptr;

    int TargetIndex = 0;
};

#endif // RENDERDIALOG_H