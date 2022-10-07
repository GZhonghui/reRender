#ifndef SERVER_PROCESSOR_H
#define SERVER_PROCESSOR_H

#include <QThread>

#include <QTcpSocket>

#include "ToolAIO.h"

#include "RenderMessage.pb.h"

class ServerProcessor : public QThread
{
    Q_OBJECT

private:
    virtual void run() override;

private:
    int RenderWidth;
    int RenderHeight;
    int Spp;
    int Left;
    int Right;
    PSceneRequest* SceneData = nullptr;

public:
    void InitRenderCore(int W, int H, int S, int L, int R, PSceneRequest* SceneData)
    {
        RenderWidth = W;
        RenderHeight = H;
        Spp = S;
        Left = L;
        Right = R;

        this->SceneData = SceneData;
    }

signals:
    void RenderedPixel(int X,int Y, QVector3D Color);
};

#endif