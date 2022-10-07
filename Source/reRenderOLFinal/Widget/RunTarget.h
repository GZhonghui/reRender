#ifndef RUNTARGET_H
#define RUNTARGET_H

#include <QObject>
#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

#include <QTcpSocket>

#include <QTime>

#include "ToolAIO.h"

#include "RenderMessage.pb.h"

class RunTarget : public QWidget
{
    Q_OBJECT
public:
    explicit RunTarget(int ID, int Width, int Height, PSceneRequest* SceneData, QWidget *parent = nullptr);
    virtual ~RunTarget();

signals:
    void RemoveSelf(int ID);
    void RenderFinish(float* Buffer, int Spp);

private:
    int TargetID = 0;

    PSceneRequest* SceneData = nullptr;

    int RenderWidth;
    int RenderHeight;

    int RecvedCount;
    std::unique_ptr<float[]> RecvedRenderResult;

private:
    QLineEdit* InputIp;
    QLineEdit* InputPort;
    QLineEdit* InputCore;
    QLineEdit* InputSpp;

    QLabel* ConnectionStatus;
    QLabel* SpendTime;

    QProgressBar* RenderProgress;

public:
    void PushConnectButton();
    void PushRemoveButton();

private:
    bool Connected = false;
    
    QString ServerIp;
    int ServerPort;
    int ServerCore;
    int ServerSpp;

    QTcpSocket* Server = nullptr;

    std::vector<uint8_t> ReadedBuffer;

public:
    void OnReadyRead();

public:
    int GetID() const { return TargetID; }
    int GetSpp() const { return ServerSpp; }
    bool GetConnected() const { return Connected; }
    bool GetComplated() const { return RecvedCount == RenderWidth * RenderHeight; }

public:
    void StartRender();
};

#endif // RUNTARGET_H
