#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "ToolAIO.h"
#include "MathAIO.h"

#include "RenderCore.h"

namespace ServerCore
{
    struct Header
    {
        int32_t dataSize;
        int32_t meshCnt;
        int32_t sphereCnt;

        void Clear()
        {
            dataSize = meshCnt = sphereCnt = 0;
        }
    };

    class ServerInstance : public QObject
    {
        Q_OBJECT
    public:
        explicit ServerInstance(QTcpServer* TCPServer, QObject *parent = nullptr)
            :QObject(parent), m_TCPServer(TCPServer)
        {
            m_Got.Clear();
        }
        virtual ~ServerInstance() {}

    protected:
        QTcpServer* m_TCPServer;
        QTcpSocket* m_Client;

        Header m_Recv,m_Got;

    public:
        void newConnection()
        {
            if(m_TCPServer->hasPendingConnections())
            {
                m_Client = m_TCPServer->nextPendingConnection();
                m_TCPServer->pauseAccepting();
                connect(m_Client,&QTcpSocket::readyRead,this,&ServerInstance::readData);

                std::cout << "Connection.";
            }
        }

        void readData()
        {

        }
    };
}

#endif // SERVERCORE_H
