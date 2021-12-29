#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "MathAIO.h"
#include "ToolAIO.h"

namespace ECore
{
    enum class OType { MESH, SPHERE, CAMERA };
    enum class MType { DIFFUSE, METAL, GLASS, EMIT };

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

#endif // ENGINE_CORE_H