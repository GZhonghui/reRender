#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "ToolAIO.h"
#include "MathAIO.h"

#include "Sync.h"

#include "RenderCore.h"

namespace ServerCore
{
    class ServerInstance : public QObject
    {
        Q_OBJECT
    public:
        explicit ServerInstance(QTcpServer* TCPServer, QObject *parent = nullptr)
            :QObject(parent), m_TCPServer(TCPServer)
        {
            m_Got.Clear();

            m_RenderEngine = std::make_unique<RenderCore::RenderEngine>();
        }
        virtual ~ServerInstance() {}

    protected:
        QTcpServer* m_TCPServer; // Not managed by this class
        QTcpSocket* m_Client;

        Sync::Header m_Recv,m_Got;

        std::vector<Sync::MeshData> m_Meshs;
        std::vector<Sync::SphereData> m_Spheres;

    private:
        std::unique_ptr<RenderCore::RenderEngine> m_RenderEngine;

    public:
        void newConnection()
        {
            if(m_TCPServer->hasPendingConnections())
            {
                m_Client = m_TCPServer->nextPendingConnection();
                m_TCPServer->pauseAccepting();
                connect(m_Client,&QTcpSocket::readyRead,this,&ServerInstance::readData);

                Out::Log(pType::MESSAGE, "Connection.");
            }
        }

        void readData()
        {
            static bool recvHeader = false;

            static bool recvMeshHeader = false;

            // Recv Header
            if(!recvHeader)
            {
                if(m_Client->bytesAvailable() < (int64_t)sizeof(Sync::Header)) return;
                recvHeader = true;
                QByteArray HeaderData = m_Client->read(sizeof(Sync::Header));
                memcpy((char*)&m_Recv, HeaderData.data(), sizeof(Sync::Header));

                Out::Log(pType::MESSAGE, "Recved Header.");
                Out::Log(pType::MESSAGE, "Total Bytes: %lld", m_Recv.dataSize);
                Out::Log(pType::MESSAGE, "Total Mesh: %lld", m_Recv.meshCnt);
                Out::Log(pType::MESSAGE, "Total Sphere: %lld", m_Recv.sphereCnt);
            }
            
            // Recv Mesh
            while(m_Got.meshCnt < m_Recv.meshCnt)
            {
                if(!recvMeshHeader)
                {
                    if(m_Client->bytesAvailable() < 8) return;

                    recvMeshHeader = true;

                    Sync::MeshData newMesh;
                    QByteArray Data = m_Client->read(8);
                    memcpy((char*)&newMesh.triangleCnt, Data.data(), 8);

                    m_Meshs.push_back(newMesh);
                }
                
                // Empty Mesh is not Allowed
                while(m_Client->bytesAvailable() >= (int64_t)sizeof(Sync::TriangleData))
                {
                    Sync::TriangleData newTriangle;
                    QByteArray Data = m_Client->read(sizeof(Sync::TriangleData));
                    memcpy((char*)&newTriangle, Data.data(), sizeof(Sync::TriangleData));

                    m_Meshs.back().Data.push_back(newTriangle);
                    if((int64_t)m_Meshs.back().Data.size() >= m_Meshs.back().triangleCnt)
                    {
                        recvMeshHeader = false;
                        m_Got.meshCnt += 1; // Done
                        break;
                    }
                }

                if(recvMeshHeader) return; // Not Done
            }

            // Recv Sphere
            while(m_Got.sphereCnt < m_Recv.sphereCnt)
            {
                if(m_Client->bytesAvailable() < (int64_t)sizeof(Sync::SphereData)) return;

                Sync::SphereData newSphere;
                QByteArray Data = m_Client->read(sizeof(Sync::SphereData));
                memcpy((char*)&newSphere, Data.data(), sizeof(Sync::SphereData));

                m_Spheres.push_back(newSphere);
                m_Got.sphereCnt += 1;
            }

            Out::Log(pType::MESSAGE, "Recved Triangle: %lld",(int64_t)m_Meshs.size());
            Out::Log(pType::MESSAGE, "Rercved Sphere: %lld",(int64_t)m_Spheres.size());

            Out::Log(pType::MESSAGE, "Recv Done.");
            Out::Log(pType::MESSAGE, "Render Starting...");

            m_RenderEngine->Render();
        }
    };
}

#endif // SERVERCORE_H
