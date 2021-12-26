#ifndef RENDERCORE_H
#define RENDERCORE_H

#include "ToolAIO.h"
#include "MathAIO.h"

namespace RenderCore
{
    class RenderEngine
    {
    public:
        explicit RenderEngine() = default;
        virtual ~RenderEngine() = default;
    };
}

#endif // RENDERCORE_H
