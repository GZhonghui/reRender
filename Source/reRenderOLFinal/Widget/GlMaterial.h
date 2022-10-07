#ifndef GL_MATERIAL_H
#define GL_MATERIAL_H

#include "MathAIO.h"
#include "ToolAIO.h"

enum class GMType
{
    DIFFUSE = 1,
    METAL,
    GLASS,
    EMIT,
    FOG
};

class GlMaterial
{
public:
    GlMaterial() = default;
    virtual ~GlMaterial() = default;

protected:
    GMType Type;

public:
    GMType GetType() const { return Type; }

public:
    std::string Name;
};

class GlDiffuse : public GlMaterial
{
public:
    GlDiffuse()
    {
        Type = GMType::DIFFUSE;
        BaseColor = Color(0,0,0);
    }

public:
    Color BaseColor;
};

class GlMetal : public GlMaterial
{
public:
    GlMetal()
    {
        Type = GMType::METAL;
        BaseColor = Color(0,0,0);
    }

public:
    Color BaseColor;
    double Fuzzy;
};

class GlGlass : public GlMaterial
{
public:
    GlGlass()
    {
        Type = GMType::GLASS;
    }

public:
    double IOR;
};

class GlEmit : public GlMaterial
{
public:
    GlEmit()
    {
        Type = GMType::EMIT;
        BaseColor = Color(0,0,0);
    }

public:
    Color BaseColor;
    double Intensity;
};

class GlFog : public GlMaterial
{
public:
    GlFog()
    {
        Type = GMType::FOG;
    }
};

#endif