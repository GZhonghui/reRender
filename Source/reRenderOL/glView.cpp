#include "glView.h"

glView::glView(QWidget *parent):
    QOpenGLWidget(parent) {}

void glView::initSkybox()
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

void glView::destroySkybox()
{
    glDeleteTextures(1, &m_SkyboxTextureID);
    glDeleteProgram (m_SkyboxShaderProgramID);

    glDeleteVertexArrays(1, &m_SkyboxVAOID);
    glDeleteBuffers(1, &m_SkyboxVBOID);
} 

void glView::renderSkybox(glm::mat4* VP)
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

void glView::changeSkybox(int Which, const QImage& Image)
{
    int texWidth = Image.width(), texHeight = Image.height();//, texChannels;
    const unsigned char* texData = Image.constBits();

    // Don't Need On Cubemap
    // stbi_set_flip_vertically_on_load(false);

    // texData = stbi_load("0.png", &texWidth, &texHeight, &texChannels, 3);
    if(!texData)
    {
        qDebug() << "Load Failed";
    }else
    {
        qDebug() << "Load Done";
    }

    switch(Which)
    {
    case 0:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    case 1:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    case 2:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    case 3:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    case 4:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    case 5:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    }
    break;
    }

    // stbi_image_free(texData);
        
}

void glView::Destroy()
{
    for(auto i=m_RenderObjects.begin();i!=m_RenderObjects.end();++i)
    {
        i->Destroy();
    }
}

void glView::initializeGL()
{
    initializeOpenGLFunctions();

    m_RenderObjects.push_back(ECore::GLRenderable());
    m_RenderObjects.back().Init(this);
} 

void glView::paintGL()
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for(auto i=m_RenderObjects.begin();i!=m_RenderObjects.end();++i)
    {
        i->Render();
    }
}
