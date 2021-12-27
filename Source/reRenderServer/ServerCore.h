#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QDataStream>
#include <QByteArray>

#include "ToolAIO.h"
#include "MathAIO.h"

#include "RenderCore.h"

namespace ServerCore
{
    // Sync with Client Version
    struct Header
    {
        int64_t dataSize;
        int64_t meshCnt;
        int64_t sphereCnt;

        void Clear()
        {
            dataSize = meshCnt = sphereCnt = 0;
        }
    };

    struct TriangleData
    {
        Point A,B,C;

        std::string Str() const
        {
            char Buffer[128];
            std::string Res;

            sprintf(Buffer,"(%.6lf, %.6lf, %.6lf), ",A.x(),A.y(),A.z());
            Res += std::string(Buffer);

            sprintf(Buffer,"(%.6lf, %.6lf, %.6lf), ",B.x(),B.y(),B.z());
            Res += std::string(Buffer);

            sprintf(Buffer,"(%.6lf, %.6lf, %.6lf).", C.x(),C.y(),C.z());
            Res += std::string(Buffer);

            return Res;
        }
    };

    struct MeshData
    {
        int64_t triangleCnt;
        std::vector<TriangleData> Data;
    };

    struct SphereData
    {
        Point Center;
        double Radius;
    };
    // Sync with Client Version End

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

        std::vector<MeshData> m_Meshs;
        std::vector<SphereData> m_Spheres;

    public:
        void newConnection()
        {
            if(m_TCPServer->hasPendingConnections())
            {
                m_Client = m_TCPServer->nextPendingConnection();
                m_TCPServer->pauseAccepting();
                connect(m_Client,&QTcpSocket::readyRead,this,&ServerInstance::readData);

                std::cout << "Connection." << std::endl;
            }
        }

        void readData()
        {
            static bool recvHeader = false;

            static bool recvMeshHeader = false;

            // Recv Header
            if(!recvHeader)
            {
                if(m_Client->bytesAvailable() < sizeof(Header)) return;
                recvHeader = true;
                QByteArray HeaderData = m_Client->read(sizeof(Header));
                memcpy((char*)&m_Recv, HeaderData.data(), sizeof(Header));
                std::cout << "Recved Header." << std::endl;
                std::cout << "Total Bytes: " << m_Recv.dataSize << std::endl;
                std::cout << "Total Mesh: " << m_Recv.meshCnt << std::endl;
                std::cout << "Total Sphere: " << m_Recv.sphereCnt << std::endl;
            }
            
            // Recv Mesh
            while(m_Got.meshCnt < m_Recv.meshCnt)
            {
                if(!recvMeshHeader)
                {
                    if(m_Client->bytesAvailable() < sizeof(int64_t)) return;

                    recvMeshHeader = true;

                    MeshData newMesh;
                    QByteArray Data = m_Client->read(sizeof(int64_t));
                    memcpy((char*)&newMesh.triangleCnt, Data.data(), sizeof(int64_t));

                    m_Meshs.push_back(newMesh);
                }
                
                // Empty Mesh is not Allowed
                while(m_Client->bytesAvailable() >= sizeof(TriangleData))
                {
                    TriangleData newTriangle;
                    QByteArray Data = m_Client->read(sizeof(TriangleData));
                    memcpy((char*)&newTriangle, Data.data(), sizeof(TriangleData));

                    std::cout << "T: " << newTriangle.Str() << std::endl;

                    m_Meshs.back().Data.push_back(newTriangle);
                    if(m_Meshs.back().Data.size() >= m_Meshs.back().triangleCnt)
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
                if(m_Client->bytesAvailable() < sizeof(SphereData)) return;

                SphereData newSphere;
                QByteArray Data = m_Client->read(sizeof(SphereData));
                memcpy((char*)&newSphere, Data.data(), sizeof(SphereData));

                m_Spheres.push_back(newSphere);
                m_Got.sphereCnt += 1;

                std::cout << "S: " << newSphere.Radius << std::endl;
            }

            std::cout << "Recv " << m_Meshs.size() << " Triangle." << std::endl;
            std::cout << "Recv " << m_Spheres.size() << " Sphere." << std::endl;

            std::cout << "Recv Done." << std::endl;
        }
    };
}

#endif // SERVERCORE_H
