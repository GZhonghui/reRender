#include "engine_core.h"

namespace ECore
{
    std::unordered_map<QString,std::unique_ptr<SceneObj>> Objects;

    QImage Skyboxs[6];
}
