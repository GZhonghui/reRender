QT       += core gui widgets openglwidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    engine_core.cpp \
    gl_viewwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    wa_button.cpp \
    wa_input.cpp

HEADERS += \
    engine_core.h \
    gl_misc.h \
    gl_viewwidget.h \
    mainwindow.h \
    math_aio.h \
    shader.h \
    tool_aio.h \
    wa_button.h \
    wa_input.h

INCLUDEPATH += ../Support/Include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    qss.qrc
