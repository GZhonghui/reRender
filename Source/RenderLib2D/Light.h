#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

inline Direction Reflect(const Direction& In, const Direction& Normal)
{
    auto useNormal = Normal.normalized();

    return In - 2 * In.dot(useNormal) * useNormal;
}

inline bool Refract(const Direction& In, const Direction& Normal, double Eta, Direction& Out)
{
    auto useIn = In.normalized();
    auto useNormal = Normal.normalized();

    auto InDotNormal = useIn.dot(useNormal);

    auto k = 1.0 - Eta * Eta * (1.0 - InDotNormal * InDotNormal);
    if (k < 0.0)
    {
        return false;
    }

    auto a = Eta * InDotNormal + std::sqrt(k);
    Out.x() = Eta * useIn.x() - a * useNormal.x();
    Out.y() = Eta * useIn.y() - a * useNormal.y();
    return true;
}

inline Color BeerLambert(const Color& Absorption, double Distance)
{
    auto AbWithDis = Absorption * Distance * -1;

    return Color(exp(AbWithDis.x()), exp(AbWithDis.y()), exp(AbWithDis.z()));
}