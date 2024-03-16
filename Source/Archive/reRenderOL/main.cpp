#define TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION
#include "ToolAIO.h"

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include <QApplication>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily("Consolas");
    font.setPixelSize(14);
    a.setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
