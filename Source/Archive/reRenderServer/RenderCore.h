#ifndef RENDERCORE_H
#define RENDERCORE_H

#include "ToolAIO.h"
#include "MathAIO.h"

#include "Material.h"

namespace RenderCore
{
    class RenderEngine
    {
    public:
        explicit RenderEngine() = default;
        virtual ~RenderEngine() = default;

    public:
        void Init()
        {

        }

        void Render()
        {
            Out::Log(pType::MESSAGE, "Render Done.");
        }
    };
}

#endif // RENDERCORE_H
