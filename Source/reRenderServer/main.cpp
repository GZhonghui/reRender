#define TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION
#include "ToolAIO.h"

#include <QCoreApplication>

#include "ServerCore.h"
#include "RenderCore.h"

std::string Logo = R"(
         ______               _             _____
         | ___ \             | |           /  ___|
 _ __ ___| |_/ /___ _ __   __| | ___ _ __  \ `--.  ___ _ ____   _____ _ __
| '__/ _ \    // _ \ '_ \ / _` |/ _ \ '__|  `--. \/ _ \ '__\ \ / / _ \ '__|
| | |  __/ |\ \  __/ | | | (_| |  __/ |    /\__/ /  __/ |   \ V /  __/ |
|_|  \___\_| \_\___|_| |_|\__,_|\___|_|    \____/ \___|_|    \_/ \___|_|
)";

std::unique_ptr<QTcpServer> TCPServer;

std::unique_ptr<ServerCore::ServerInstance> ServerInstance;

const int Port = 2049;

bool initServer()
{
    TCPServer = std::make_unique<QTcpServer>();

    ServerInstance = std::make_unique<ServerCore::ServerInstance>(TCPServer.get());

    if(!TCPServer->listen(QHostAddress::Any, Port)) return false;

    QObject::connect(TCPServer.get(),&QTcpServer::newConnection,
        ServerInstance.get(),&ServerCore::ServerInstance::newConnection);

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << Logo << std::endl;

    Out::Log(pType::MESSAGE,"reRender Server, Version: 2.0, Author: GZhonghui.");
    Out::Log(pType::MESSAGE,"Initing...");

    if(initServer())
    {
        Out::Log(pType::MESSAGE, "Init Server Done, Running...");
    }else
    {
        Out::Log(pType::ERROR, "Init Server Failed, Exiting...");
        return -1;
    }

    return a.exec();
}
