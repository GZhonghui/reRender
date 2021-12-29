QT += gui network

CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp

HEADERS += \
    MathAIO.h \
    RenderCore.h \
    ServerCore.h \
    ToolAIO.h \
    Sync.h \
    Material.h

INCLUDEPATH += ../Support/Include/

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
