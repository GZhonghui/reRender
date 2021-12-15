#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#endif // ENGINE_CORE_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

#include <QImage>

namespace ECore
{
    enum OType { MESH, SPHERE, CAMERA };
    enum MType { DIFFUSE };

    class SceneObj
    {
    public:
        OType m_OType;
        MType m_MType;
        QString m_ID;
    };
}
