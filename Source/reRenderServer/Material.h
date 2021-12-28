#ifndef MATERIAL_H
#define MATERIAL_H

#include "ToolAIO.h"
#include "MathAIO.h"

namespace RenderCore
{
    enum class mType { DIFFUSE, METAL, GLASS, EMIT };

    class Material
    {
    public:
        Material(mType thisType): m_Type(thisType) {}
        virtual ~Material() = default;

    protected:
        mType m_Type;
        mType Type() const { return m_Type; }

    public:
        virtual void LoadBytes(const QByteArray& Data) = 0;
        virtual void SaveBytes(QByteArray& Data) = 0;
    };

    class Diffuse : public Material
    {
    public:
        Diffuse(): Material(mType::DIFFUSE) {}
        virtual ~Diffuse() = default;

    protected:
        Color m_BaseColor;

        bool m_EnableDiffuseTexture;
        QImage m_DiffuseTexture;
        
        bool m_EnableNormalTexture;
        QImage m_NormalTexture;

    public:
        virtual void LoadBytes(const QByteArray& Data)
        {

        }

        virtual void SaveBytes(QByteArray& Data)
        {

        }
    };

    class Metal : public Material
    {
    public:
        Metal(): Material(mType::METAL) {}
        virtual ~Metal() = default;

    protected:
        Color m_BaseColor;

    public:
        virtual void LoadBytes(const QByteArray& Data)
        {

        }

        virtual void SaveBytes(QByteArray& Data)
        {

        }
    };

    class Glass : public Material
    {
    public:
        Glass(): Material(mType::GLASS) {}
        virtual ~Glass() = default;

    protected:
        double m_IOR;
    
    public:
        virtual void LoadBytes(const QByteArray& Data)
        {

        }

        virtual void SaveBytes(QByteArray& Data)
        {

        }
    };

    class Emit : public Material
    {
    public:
        Emit(): Material(mType::EMIT) {}
        virtual ~Emit() = default;

    protected:
        Color m_BaseColor;

    public:
        virtual void LoadBytes(const QByteArray& Data)
        {

        }

        virtual void SaveBytes(QByteArray& Data)
        {

        }
    };
}

#endif // MATERIAL_H