#ifndef GL_RENDERABLE_H
#define GL_RENDERABLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "GlMisc.h"

#include "RenderMessage.pb.h"

class GlRenderable
{
public:
    GlRenderable()
    {
        m_VAOID = 0;
        m_VBOID = 0;
        m_EBOID = 0;

        m_EnableDiffuseTexture  = false;
        m_EnableNormalTexture   = false;
        m_EnableSpecularTexture = false;

        m_DiffuseTextureID  = 0;
        m_NormalTextureID   = 0;
        m_SpecularTextureID = 0;
    }
    virtual ~GlRenderable()
    {
        Clear();
    }

private:
    QOpenGLFunctions_3_3_Core* m_F;
    
public:
    void Init(QOpenGLFunctions_3_3_Core* F)
    {
        m_F = F;

        uint32_t VertShaderID = GlMisc::CompileShader(m_F,
            Shader("GLDirect", sType::VERT).m_ShaderCode.data(), sType::VERT);
        uint32_t FragShaderID = GlMisc::CompileShader(m_F,
            Shader("GLDirect", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

        m_DirectShaderProgramID = m_F->glCreateProgram();
        m_F->glAttachShader(m_DirectShaderProgramID, VertShaderID);
        m_F->glAttachShader(m_DirectShaderProgramID, FragShaderID);
        m_F->glLinkProgram (m_DirectShaderProgramID);

        m_F->glDeleteShader(VertShaderID);
        m_F->glDeleteShader(FragShaderID);
    }

private:
    uint32_t m_DirectShaderProgramID; // Just for debug

public:
    void LoadCube() // Just for debug
    {
        if (m_VAOID) m_F->glDeleteVertexArrays(1, &m_VAOID);
        if (m_VBOID) m_F->glDeleteBuffers(1, &m_VBOID);
        if (m_EBOID) m_F->glDeleteBuffers(1, &m_EBOID);

        m_F->glGenVertexArrays(1, &m_VAOID);
        m_F->glGenBuffers(1, &m_EBOID);
        m_F->glGenBuffers(1, &m_VBOID);

        /* With UV, 36 Vertices
        float Vertices[] =
        {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        */

        float Vertices[] =
        {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        std::vector<uint32_t> m_IndexData;
        m_IndexData.resize(36);
        for(uint32_t i=0; i<36; ++i) m_IndexData[i]=i;

        m_F->glBindVertexArray(m_VAOID);
        m_F->glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
        m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        m_F->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOID);
        m_F->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*m_IndexData.size(), m_IndexData.data(), GL_STATIC_DRAW);
        m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_F->glEnableVertexAttribArray(0);

        m_F->glBindBuffer(GL_ARRAY_BUFFER, 0); 
        m_F->glBindVertexArray(0);

        m_ElementCount = (int)m_IndexData.size();
    }

protected:
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

    Color m_DiffuseColor;

    bool m_EnableDiffuseTexture;
    bool m_EnableNormalTexture;
    bool m_EnableSpecularTexture;

    uint32_t m_DiffuseTextureID;
    uint32_t m_NormalTextureID;
    uint32_t m_SpecularTextureID;

    std::string m_NowModel;
    std::string m_NowDiffuseTexture;
    std::string m_NowNormalTexture;
    std::string m_NowSpecularTexture;

    double m_N;

protected:
    std::string m_Shader;

    unsigned int m_ElementCount;

public:
    bool isTriangle = true;

public:
    void  setDiffuseColor(const Color& newColor) { m_DiffuseColor = newColor; }
    Color getDiffuseColor() const noexcept       { return m_DiffuseColor;     }

    void   setN(double newN) { m_N = newN; }
    double getN() const      { return m_N; }

    void setShader(const std::string& newShader) { m_Shader = newShader; }

    uint32_t getDiffuseTextureID()  const { return m_DiffuseTextureID;  }
    uint32_t getNormalTextureID()   const { return m_NormalTextureID;   }
    uint32_t getSpecularTextureID() const { return m_SpecularTextureID; }

    void EnableDiffuseTexture (bool Yes) { m_EnableDiffuseTexture  = Yes; }
    void EnableNormalTexture  (bool Yes) { m_EnableNormalTexture   = Yes; }
    void EnableSpecularTexture(bool Yes) { m_EnableSpecularTexture = Yes; }

public:
    void reLoadModel(const std::string& Model);
    void reLoadDiffuseTexture (const std::string& DiffuseTextureName );
    void reLoadNormalTexture  (const std::string& NormalTextureName  );
    void reLoadSpecularTexture(const std::string& SpecularTextureName);

public:
    void reLoadSphere();

public:
    void Clear();

public:
    void Draw();

public:
    std::vector<PSceneRequest_Triangle> m_Triangles;

public:
    friend class GlView;
    friend class GlManager;
};

#endif // GL_RENDERABLE_H