#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication App(argc, argv);

    QFont Font;
    Font.setFamily("Consolas");
    Font.setPixelSize(14);
    App.setFont(Font);

    MainWindow Window;
    Window.show();

    return App.exec();
}
