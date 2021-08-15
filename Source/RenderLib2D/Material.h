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
        Material(mType::EMISSION), m_EmissionColor(EmissionColor) {}
    virtual ~Emission() = default;

public:
    Color getEmissionColor() const noexcept
    {
        return m_EmissionColor;
    }
};

class Glass : public Material
{
protected:
    double m_Reflectivity;
    double m_Eta;
    Color m_Absorption;
    Color m_EmissionColor;

public:
    Glass
    (
        double Reflectivity, double Eta, const Color& Absorption,
        const Color& EmissionColor = Color(0, 0, 0)
    ) :
        Material(mType::GLASS), m_Reflectivity(Reflectivity), m_Eta(Eta),
        m_Absorption(Absorption), m_EmissionColor(EmissionColor) {}
    virtual ~Glass() = default;

public:
    double getReflectivity() const noexcept
    {
        return m_Reflectivity;
    }

    double getEta() const noexcept
    {
        return m_Eta;
    }

    Color getAbsorption() const noexcept
    {
        return m_Absorption;
    }

    Color getEmissionColor() const noexcept
    {
        return m_EmissionColor;
    }
};