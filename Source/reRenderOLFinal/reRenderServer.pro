# Qt Version: 6.3.0

QT += core gui widgets network

CONFIG += c++17
CONFIG += console

SOURCES += \
    Protobuf/RenderMessage.pb.cc \
    Misc/GTexture.cpp \
    Thread/ServerProcessor.cpp \
    Thread/TcpServer.cpp \
    MainServer.cpp

HEADERS += \
    Core/common/rtweekend.h \
    Core/common/camera.h \
    Core/common/ray.h \
    Core/common/vec3.h \
    Core/common/aabb.h \
    Core/common/external/stb_image.h \
    Core/common/perlin.h \
    Core/common/rtw_stb_image.h \
    Core/common/texture.h \
    Core/TheRestOfYourLife/aarect.h \
    Core/TheRestOfYourLife/box.h \
    Core/TheRestOfYourLife/bvh.h \
    Core/TheRestOfYourLife/hittable.h \
    Core/TheRestOfYourLife/hittable_list.h \
    Core/TheRestOfYourLife/material.h \
    Core/TheRestOfYourLife/onb.h \
    Core/TheRestOfYourLife/pdf.h \
    Core/TheRestOfYourLife/sphere.h \
    Core/Scene.h \
    Misc/MathAIO.h \
    Misc/ToolAIO.h \
    Misc/GTexture.h \
    Misc/GSkybox.h \
    Protobuf/RenderMessage.pb.h \
    Thread/ServerProcessor.h \
    Thread/TcpServer.h

INCLUDEPATH += Core
INCLUDEPATH += Misc
INCLUDEPATH += Protobuf
INCLUDEPATH += Thread

# Ray Tracing
INCLUDEPATH += Core/common
INCLUDEPATH += Core/TheRestOfYourLife

# 3rd Party Libs
INCLUDEPATH += Support/Include

LIBS += -L$$PWD/Support/Lib -lprotobuf

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
