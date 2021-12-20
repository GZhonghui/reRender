#ifndef GL_RENDERABLE_H
#define GL_RENDERABLE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "shader.h"

class GLRenderable
{
public:
    GLRenderable() = default;
    ~GLRenderable() { Destroy(); }

private:
    QOpenGLFunctions_3_3_Core* m_F;

public:
    uint32_t m_ShaderProgramID;
    uint32_t m_VAOID;
    uint32_t m_VBOID;

public:
    QOpenGLShaderProgram* m_Shader;
    QOpenGLVertexArrayObject* m_VAO;
    QOpenGLBuffer* m_VBO;

public:
    void Init(QOpenGLFunctions_3_3_Core* F)
    {
        m_F = F;
        
        uint32_t VertShaderID = GLMisc::CompileShader(m_F, Shader("Direct", sType::VERT).m_ShaderCode.data(), sType::VERT);
        uint32_t FragShaderID = GLMisc::CompileShader(m_F, Shader("Direct", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

        m_ShaderProgramID = m_F->glCreateProgram();
        m_F->glAttachShader(m_ShaderProgramID, VertShaderID);
        m_F->glAttachShader(m_ShaderProgramID, FragShaderID);
        m_F->glLinkProgram (m_ShaderProgramID);

        m_F->glDeleteShader(VertShaderID);
        m_F->glDeleteShader(FragShaderID);
        

        float Vertices[] =
        {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        m_F->glGenVertexArrays(1, &m_VAOID);
        m_F->glGenBuffers(1, &m_VBOID);

        m_F->glBindVertexArray(m_VAOID);
        m_F->glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
        m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_F->glEnableVertexAttribArray(0);

        m_F->glBindBuffer(GL_ARRAY_BUFFER, 0); 
        m_F->glBindVertexArray(0);

        m_Shader = new QOpenGLShaderProgram();
        m_Shader->addShaderFromSourceCode(QOpenGLShader::Vertex, Shader("Direct", sType::VERT).m_ShaderCode.data());
        m_Shader->addShaderFromSourceCode(QOpenGLShader::Fragment, Shader("Direct", sType::FRAG).m_ShaderCode.data());
        m_Shader->link();

        m_VAO = new QOpenGLVertexArrayObject();
        m_VBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

        m_VAO->create();
        m_VAO->bind();

        m_VBO->create();
        m_VBO->bind();
        m_VBO->allocate(Vertices, 3*3*sizeof(float));

        m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_F->glEnableVertexAttribArray(0);

        m_VBO->release();
        m_VAO->release();
    }

    void Destroy()
    {
        // m_F->glDeleteProgram(m_ShaderProgramID);

        // m_F->glDeleteVertexArrays(1, &m_VAOID);
        // m_F->glDeleteBuffers(1, &m_VBOID);
    }

    void Render()
    {
        // m_F->glUseProgram(m_ShaderProgramID);
        // m_F->glBindVertexArray(m_VAOID);

        // m_VAO->bind();
        // m_Shader->bind();

        m_F->glUseProgram(m_ShaderProgramID);
        m_F->glBindVertexArray(m_VAOID);
        m_F->glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

#endif // GL_RENDERABLE_H
