QT += core gui widgets openglwidgets network

CONFIG += c++17

SOURCES += \
    color_button.cpp \
    engine_core.cpp \
    gl_viewwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    wa_button.cpp \
    wa_input.cpp

HEADERS += \
    MathAIO.h \
    ToolAIO.h \
    camera.h \
    color_button.h \
    engine_core.h \
    gl_misc.h \
    gl_renderable.h \
    gl_viewwidget.h \
    mainwindow.h \
    shader.h \
    wa_button.h \
    wa_input.h

INCLUDEPATH += ../Support/Include/

RESOURCES += \
    img.qrc \
    qss.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
