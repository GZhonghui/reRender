# Qt Version: 6.3.0

QT += core gui widgets openglwidgets network

CONFIG += c++17
CONFIG += console

SOURCES += \
    Protobuf/RenderMessage.pb.cc \
    Widget/GlView.cpp \
    Widget/GlRenderable.cpp \
    Widget/RunTarget.cpp \
    Editor/MainWindow.cpp \
    Editor/RenderDialog.cpp \
    MainClient.cpp

HEADERS += \
    Core/Scene.h \
    Misc/MathAIO.h \
    Misc/ToolAIO.h \
    Misc/Loader.h \
    Misc/Shader.h \
    Protobuf/RenderMessage.pb.h \
    Widget/GlMisc.h \
    Widget/GlView.h \
    Widget/GlCamera.h \
    Widget/GlMaterial.h \
    Widget/GlRenderable.h \
    Widget/RunTarget.h \
    Editor/MainWindow.h \
    Editor/RenderDialog.h

RESOURCES += \
    Qrc/Image.qrc \
    Qrc/Qss.qrc

INCLUDEPATH += Core
INCLUDEPATH += Editor
INCLUDEPATH += Misc
INCLUDEPATH += Protobuf
INCLUDEPATH += Widget

# 3rd Party Libs
INCLUDEPATH += Support/Include

LIBS += -L$$PWD/Support/Lib -lprotobuf -lassimp -lzlibstatic
LIBS += -lOpenImageDenoise

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
