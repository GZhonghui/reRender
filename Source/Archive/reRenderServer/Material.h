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
        Diffuse(): Material(mType::DIFFUSE),
            m_EnableDiffuseTexture(0), m_EnableNormalTexture(0) {}
        virtual ~Diffuse() = default;

    protected:
        Color m_BaseColor;

        int16_t m_EnableDiffuseTexture;
        int64_t m_DiffuseTextureWidth;
        int64_t m_DiffuseTextureHeight;
        QImage m_DiffuseTexture;
        
        int16_t m_EnableNormalTexture;
        int64_t m_NormalTextureWidth;
        int64_t m_NormalTextureHeight;
        QImage m_NormalTexture;

    public:
        void setBaseColor(const Color& newColor) { m_BaseColor = newColor; }
        Color BaseColor() const { return m_BaseColor; }

        void setDiffuseTexture(const QImage& Texture)
        {
            m_EnableDiffuseTexture = 1;
            m_DiffuseTextureWidth  = Texture.width();
            m_DiffuseTextureHeight = Texture.height();
            m_DiffuseTexture = Texture.convertToFormat(QImage::Format_RGB888);
        }

        void clearDiffuseTexture() { m_EnableDiffuseTexture = 0; }
        QImage DiffuseTexture() const { return m_DiffuseTexture; }

    public:
        virtual void LoadBytes(const QByteArray& Data)
        {
            unsigned int Index = 0;
            auto Copy = [&](char* Target, int Size)
            {
                memcpy(Target, Data.data() + Index, Size);
                Index += Size;
            };

            Copy((char*)&m_BaseColor, sizeof(Color));

            Copy((char*)&m_EnableDiffuseTexture,sizeof(int16_t));
            Copy((char*)&m_DiffuseTextureWidth, sizeof(int64_t));
            Copy((char*)&m_DiffuseTextureHeight,sizeof(int64_t));
            if(m_EnableDiffuseTexture)
            {
                m_DiffuseTexture = QImage((uchar*)Data.constData() + Index,
                    m_DiffuseTextureWidth, m_DiffuseTextureHeight, QImage::Format_RGB888);
                Index += m_DiffuseTextureWidth*m_DiffuseTextureHeight*3;
            }

            Copy((char*)&m_EnableNormalTexture,sizeof(int16_t));
            Copy((char*)&m_NormalTextureWidth, sizeof(int64_t));
            Copy((char*)&m_NormalTextureHeight,sizeof(int64_t));
            if(m_EnableNormalTexture)
            {
                m_NormalTexture = QImage((uchar*)Data.constData() + Index,
                    m_NormalTextureWidth, m_NormalTextureHeight, QImage::Format_RGB888);
                Index += m_NormalTextureWidth*m_NormalTextureHeight*3;
            }
        }

        virtual void SaveBytes(QByteArray& Data)
        {
            int dataSize = sizeof(Color)
                + sizeof(int16_t) * 2 
                + sizeof(int64_t) * 4
                + (m_EnableDiffuseTexture ? m_DiffuseTextureWidth * m_DiffuseTextureHeight * 3 : 0)
                + (m_EnableNormalTexture  ? m_NormalTextureWidth  * m_NormalTextureHeight  * 3 : 0);
            Data.resize(dataSize);

            unsigned int Index = 0;
            
            auto Copy = [&](const char* Source, int Size)
            {
                memcpy(Data.data() + Index, Source, Size);
                Index += Size;
            };
            Copy((char*)&m_BaseColor,sizeof(Color));
            
            Copy((char*)&m_EnableDiffuseTexture,sizeof(int16_t));
            Copy((char*)&m_DiffuseTextureWidth, sizeof(int64_t));
            Copy((char*)&m_DiffuseTextureHeight,sizeof(int64_t));
            if(m_EnableDiffuseTexture)
            {
                Copy((char*)m_DiffuseTexture.constBits(), m_DiffuseTextureWidth*m_DiffuseTextureHeight*3);
            }
                
            Copy((char*)&m_EnableNormalTexture,sizeof(int16_t));
            Copy((char*)&m_NormalTextureWidth, sizeof(int64_t));
            Copy((char*)&m_NormalTextureHeight,sizeof(int64_t));
            if(m_EnableNormalTexture)
            {
                Copy((char*)m_NormalTexture.constBits(), m_NormalTextureWidth*m_NormalTextureHeight*3);
            }
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
