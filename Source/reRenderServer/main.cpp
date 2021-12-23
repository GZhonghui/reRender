#include <QCoreApplication>

#include <iostream>
#include <string>

std::string Logo = R"(
         ______               _             _____
         | ___ \             | |           /  ___|
 _ __ ___| |_/ /___ _ __   __| | ___ _ __  \ `--.  ___ _ ____   _____ _ __
| '__/ _ \    // _ \ '_ \ / _` |/ _ \ '__|  `--. \/ _ \ '__\ \ / / _ \ '__|
| | |  __/ |\ \  __/ | | | (_| |  __/ |    /\__/ /  __/ |   \ V /  __/ |
|_|  \___\_| \_\___|_| |_|\__,_|\___|_|    \____/ \___|_|    \_/ \___|_|
)";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << Logo << std::endl;

    std::cout << "reRender Server, Version: 2.0." << std::endl;
    std::cout << "Initing..." << std::endl;


    return a.exec();
}
