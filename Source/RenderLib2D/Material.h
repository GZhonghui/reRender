#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

enum class mType
{
    EMISSION, GLASS
};

class Material
{
protected:
    mType m_Type;

public:
    Material(mType Type) : m_Type(Type) {}
    virtual ~Material() = default;

public:
    mType getType() const noexcept { return m_Type; }
};

class Emission : public Material
{
protected:
    Color m_EmissionColor;

public:
    Emission(const Color& EmissionColor) :
        m_EmissionColor(EmissionColor), Material(mType::EMISSION) {}
    virtual ~Emission() = default;

public:
    Color getEmissionColor() const noexcept
    {
        return m_EmissionColor;
    }
};

class Glass : public Material
{
public:
    Glass() : Material(mType::GLASS) {}
    virtual ~Glass() = default;
};