#include <QCoreApplication>

/*
#include <QtNetwork>

#include <QTcpServer>
#include <QHostAddress>


#include <iostream>
#include <string>
#include <memory>

std::string Logo = R"(
         ______               _             _____
         | ___ \             | |           /  ___|
 _ __ ___| |_/ /___ _ __   __| | ___ _ __  \ `--.  ___ _ ____   _____ _ __
| '__/ _ \    // _ \ '_ \ / _` |/ _ \ '__|  `--. \/ _ \ '__\ \ / / _ \ '__|
| | |  __/ |\ \  __/ | | | (_| |  __/ |    /\__/ /  __/ |   \ V /  __/ |
|_|  \___\_| \_\___|_| |_|\__,_|\___|_|    \____/ \___|_|    \_/ \___|_|
)";

std::unique_ptr<QTcpServer> Server;

bool initServer()
{
    // Server = std::make_unique<QTcpServer>();

    // if(!Server->listen(QHostAddress::Any, Port)) return false;

    return true;
}

void newConnection()
{

}
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
/*
    std::cout << Logo << std::endl;

    std::cout << "reRender Server, Version: 2.0, Author: GZhonghui." << std::endl;
    std::cout << "Initing..." << std::endl;

    if(initServer())
    {
        std::cout << "Init Server Done, Running..." << std::endl;
    }else
    {
        std::cout << "Exiting..." << std::endl;
    }
*/
    // Server->close();

    return a.exec();
}
