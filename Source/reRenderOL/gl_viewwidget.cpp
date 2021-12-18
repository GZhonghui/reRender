#include "gl_viewwidget.h"

GL_ViewWidget::GL_ViewWidget(QWidget *parent):
    QOpenGLWidget(parent) {}

void GL_ViewWidget::initSkybox()
{
    // Create Texture of Skybox
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Load and Complie Shader
    uint32_t skyboxVertShaderID = GLMisc::CompileShader(this, Shader("Skybox", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t skyboxFragShaderID = GLMisc::CompileShader(this, Shader("Skybox", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_SkyboxShaderProgramID = glCreateProgram();
    glAttachShader(m_SkyboxShaderProgramID, skyboxVertShaderID);
    glAttachShader(m_SkyboxShaderProgramID, skyboxFragShaderID);
    glLinkProgram (m_SkyboxShaderProgramID);

    glDeleteShader(skyboxVertShaderID);
    glDeleteShader(skyboxFragShaderID);

    // Data of Skybox Cube
    float skyboxVertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_SkyboxVAOID);
    glGenBuffers(1, &m_SkyboxVBOID);

    glBindVertexArray(m_SkyboxVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GL_ViewWidget::destroySkybox()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram (m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
}

void GL_ViewWidget::changeSkybox(int Which, const QPixmap& Image)
{

}

void GL_ViewWidget::renderSkybox(glm::mat4* VP)
{
    glDepthMask(GL_FALSE);

    glUseProgram(m_SkyboxShaderProgramID);

    // Skybox's View is Different
    auto skyboxV = glm::mat4(glm::mat3(VP[0]));

    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(skyboxV));
    glUniformMatrix4fv(glGetUniformLocation(m_SkyboxShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(VP[1]));

    glUniform1i(glGetUniformLocation(m_SkyboxShaderProgramID, "skyboxTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    glBindVertexArray(m_SkyboxVAOID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}

void GL_ViewWidget::Destroy()
{
    destroySkybox();
}

void GL_ViewWidget::initializeGL()
{
    initializeOpenGLFunctions();
    
    initSkybox();
}

void GL_ViewWidget::paintGL()
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glm::mat4 VP[2];

    VP[0] = glm::mat4(1.0);

    VP[1] = glm::perspective(glm::radians(100.0), (double)1024 / 768, 0.1, 100.0);

    renderSkybox(VP);
}
