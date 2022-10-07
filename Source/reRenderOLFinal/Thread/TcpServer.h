#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <cstdio>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include <QObject>

#include "ToolAIO.h"

#include "GSkybox.h"

#include "ServerProcessor.h"

#include "RenderMessage.pb.h"

class TcpServer : public QObject
{
    Q_OBJECT

public:
    TcpServer();
    virtual ~TcpServer();

public:
    void OnConnect();
    void OnReadyRead();
    void OnStateChanged(QAbstractSocket::SocketState SocketState);

private:
    QTcpServer  Server;
    QTcpSocket* Client = nullptr;

    std::vector<uint8_t> ReadedBuffer;

    std::vector<ServerProcessor*> RenderThreads;

private:
    int RenderWidth;
    int RenderHeight;
    int Core;
    int Spp;

    PSceneRequest SceneData;
    
public:
    void OnRenderedPixel(int X, int Y, QVector3D Color);
};

#endif