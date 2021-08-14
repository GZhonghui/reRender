#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

inline Direction Reflect(const Direction& In, const Direction& Normal)
{
    auto useNormal = Normal.normalized();

    return In - 2 * In.dot(useNormal) * useNormal;
}

