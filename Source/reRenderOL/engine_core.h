#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#endif // ENGINE_CORE_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

#include <QImage>

#include "tool_aio.h"

namespace ECore
{
    enum OType { MESH, SPHERE, CAMERA };
    enum MType { DIFFUSE, METAL, GLASS, EMIT };

    class SceneObj
    {
    public:
        OType m_OType;
        MType m_MType;
        QString m_ID;
    };

    #ifdef DEF_ENGINE_CORE
    std::unordered_map<QString,std::unique_ptr<SceneObj>> Objects;
    QImage Skyboxs[6];

    #else
    extern std::unordered_map<QString,std::unique_ptr<SceneObj>> Objects;
    extern QImage Skyboxs[6];

    #endif // DEF_ENGINE_CORE
}
