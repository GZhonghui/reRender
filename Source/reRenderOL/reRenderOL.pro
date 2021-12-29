QT += core gui widgets openglwidgets network

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainWindow.cpp \
    waButton.cpp \
    waInput.cpp \
    colorButton.cpp \
    glView.cpp \ 
    engine_core.cpp

HEADERS += \
    MathAIO.h \
    ToolAIO.h \
    mainWindow.h \
    Camera.h \
    Shader.h \
    Sync.h \
    glMisc.h \
    glRenderable.h \
    waButton.h \
    waInput.h \
    colorButton.h \
    glView.h \
    engine_core.h

INCLUDEPATH += ../Support/Include/

RESOURCES += \
    img.qrc \
    qss.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
