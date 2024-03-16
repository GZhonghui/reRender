#include "GlView.h"

void GlManager::Skybox::Init()
{
    // HARD CODE
    m_Skyboxs.push_back("Creek");
    m_SkyboxsPath.push_back("./Asset/Skybox/Creek/");
    m_RevSkyboxs[std::string("Creek")] = 0;

    m_Skyboxs.push_back("Water");
    m_SkyboxsPath.push_back("./Asset/Skybox/Water/");
    m_RevSkyboxs[std::string("Water")] = 1;

    m_SkyboxsChar = "Creek\0Water\0";
    // HARD CODE

    m_F->glGenTextures(1, &m_SkyboxTextureID);
    m_F->glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    m_F->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_F->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_F->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_F->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_F->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_NowSkybox = -1;
    Change(0);

    uint32_t skyboxVertShaderID = GlMisc::CompileShader(m_F, Shader("GLSkybox", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t skyboxFragShaderID = GlMisc::CompileShader(m_F, Shader("GLSkybox", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_SkyboxShaderProgramID = m_F->glCreateProgram();
    m_F->glAttachShader(m_SkyboxShaderProgramID, skyboxVertShaderID);
    m_F->glAttachShader(m_SkyboxShaderProgramID, skyboxFragShaderID);
    m_F->glLinkProgram (m_SkyboxShaderProgramID);

    m_F->glDeleteShader(skyboxVertShaderID);
    m_F->glDeleteShader(skyboxFragShaderID);

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

    m_F->glGenVertexArrays(1, &m_SkyboxVAOID);
    m_F->glGenBuffers(1, &m_SkyboxVBOID);

    m_F->glBindVertexArray(m_SkyboxVAOID);
    m_F->glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBOID);
    m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_F->glEnableVertexAttribArray(0);
}

void GlManager::Skybox::Destroy()
{
    m_F->glDeleteTextures(1, &m_SkyboxTextureID);
    m_F->glDeleteProgram (m_SkyboxShaderProgramID);

    m_F->glDeleteVertexArrays(1, &m_SkyboxVAOID);
    m_F->glDeleteBuffers(1, &m_SkyboxVBOID);
}

void GlManager::Skybox::Render(glm::mat4* VP)
{
    m_F->glDepthMask(GL_FALSE);

    m_F->glUseProgram(m_SkyboxShaderProgramID);

    // Skybox's View is Different
    auto skyboxV = glm::mat4(glm::mat3(VP[0]));

    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_SkyboxShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(skyboxV));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_SkyboxShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(VP[1]));

    m_F->glUniform1i(m_F->glGetUniformLocation(m_SkyboxShaderProgramID, "skyboxTexture"), 0);

    m_F->glActiveTexture(GL_TEXTURE0);
    m_F->glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTextureID);

    m_F->glBindVertexArray(m_SkyboxVAOID);
    m_F->glDrawArrays(GL_TRIANGLES, 0, 36);

    m_F->glDepthMask(GL_TRUE);
}

void GlManager::Skybox::Change(int Which)
{
    if (!Inside(Which, 0, m_Skyboxs.size() - 1)) return;
    if (m_NowSkybox == Which) return;

    std::string SkyboxPath = m_SkyboxsPath[Which];

    int texWidth, texHeight, texChannels;
    unsigned char* texData;

    // Don't Need On Cubemap
    stbi_set_flip_vertically_on_load(false);
    texData = stbi_load((SkyboxPath + "posX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negX.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negY.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "posZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);

    texData = stbi_load((SkyboxPath + "negZ.jpg").c_str(), &texWidth, &texHeight, &texChannels, 3);
    m_F->glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);
    stbi_set_flip_vertically_on_load(true);

    m_NowSkybox = Which;
}

void GlManager::Grid::Init()
{
    uint32_t groundVertShaderID = GlMisc::CompileShader(m_F, Shader("GLLine", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t groundFragShaderID = GlMisc::CompileShader(m_F, Shader("GLLine", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_GridShaderProgramID = m_F->glCreateProgram();
    m_F->glAttachShader(m_GridShaderProgramID, groundVertShaderID);
    m_F->glAttachShader(m_GridShaderProgramID, groundFragShaderID);
    m_F->glLinkProgram (m_GridShaderProgramID);

    m_F->glDeleteShader(groundVertShaderID);
    m_F->glDeleteShader(groundFragShaderID);

    m_F->glGenVertexArrays(1, &m_GridVAOID);
    m_F->glGenBuffers(1, &m_GridVBOID);

    std::vector<float> V;

    const int   GridSize   = 40;
    const float GridOffset = 0.5;
    const float GridStart  =  GridSize * -0.5 * GridOffset;
    const float GridEnd    = -GridStart;
    
    for (int wIndex = 0; wIndex <= GridSize; wIndex += 1)
    {
        V.push_back(GridStart + wIndex * GridOffset);
        V.push_back(0);
        V.push_back(GridStart);

        V.push_back(GridStart + wIndex * GridOffset);
        V.push_back(0);
        V.push_back(GridEnd);
    }

    for (int hIndex = 0; hIndex <= GridSize; hIndex += 1)
    {
        V.push_back(GridStart);
        V.push_back(0);
        V.push_back(GridStart + hIndex * GridOffset);
        
        V.push_back(GridEnd);
        V.push_back(0);
        V.push_back(GridStart + hIndex * GridOffset);
    }

    m_GridVerticesCount = V.size() / 3;

    m_F->glBindVertexArray(m_GridVAOID);
    m_F->glBindBuffer(GL_ARRAY_BUFFER, m_GridVBOID);
    m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * V.size(), V.data(), GL_STATIC_DRAW);

    m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_F->glEnableVertexAttribArray(0);
}

void GlManager::Grid::Destroy()
{
    m_F->glDeleteProgram(m_GridShaderProgramID);

    m_F->glDeleteVertexArrays(1, &m_GridVAOID);
    m_F->glDeleteBuffers(1, &m_GridVBOID);
}

void GlManager::Grid::Render(glm::mat4* VP)
{
    m_F->glUseProgram(m_GridShaderProgramID);

    auto M = glm::mat4(1);

    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_GridShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(M));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_GridShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(VP[0]));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_GridShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(VP[1]));
    m_F->glUniform3f(m_F->glGetUniformLocation(m_GridShaderProgramID, "lineColor"), colorGray.x(), colorGray.y(), colorGray.z());

    m_F->glLineWidth(1.0);

    m_F->glBindVertexArray(m_GridVAOID);
    m_F->glDrawArrays(GL_LINES, 0, m_GridVerticesCount);
}

void GlManager::Axis::Init()
{
    uint32_t axisEndVertShaderID = GlMisc::CompileShader(m_F, Shader("GLAxisEnd", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t axisEndFragShaderID = GlMisc::CompileShader(m_F, Shader("GLAxisEnd", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_AxisEndShaderProgramID = m_F->glCreateProgram();
    m_F->glAttachShader(m_AxisEndShaderProgramID, axisEndVertShaderID);
    m_F->glAttachShader(m_AxisEndShaderProgramID, axisEndFragShaderID);
    m_F->glLinkProgram (m_AxisEndShaderProgramID);

    m_F->glDeleteShader(axisEndVertShaderID);
    m_F->glDeleteShader(axisEndFragShaderID);

    uint32_t axisLineVertShaderID = GlMisc::CompileShader(m_F, Shader("GLAxisLine", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t axisLineFragShaderID = GlMisc::CompileShader(m_F, Shader("GLAxisLine", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_AxisLineShaderProgramID = m_F->glCreateProgram();
    m_F->glAttachShader(m_AxisLineShaderProgramID, axisLineVertShaderID);
    m_F->glAttachShader(m_AxisLineShaderProgramID, axisLineFragShaderID);
    m_F->glLinkProgram (m_AxisLineShaderProgramID);

    m_F->glDeleteShader(axisLineVertShaderID);
    m_F->glDeleteShader(axisLineFragShaderID);

    float boxVertices[] =
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

    m_F->glGenVertexArrays(1, &m_AxisEndVAOID);
    m_F->glGenBuffers(1, &m_AxisEndVBOID);

    m_F->glBindVertexArray(m_AxisEndVAOID);
    m_F->glBindBuffer(GL_ARRAY_BUFFER, m_AxisEndVBOID);
    m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

    m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_F->glEnableVertexAttribArray(0);

    float lineVertices[] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    m_F->glGenVertexArrays(1, &m_AxisLineVAOID);
    m_F->glGenBuffers(1, &m_AxisLineVBOID);

    m_F->glBindVertexArray(m_AxisLineVAOID);
    m_F->glBindBuffer(GL_ARRAY_BUFFER, m_AxisLineVBOID);
    m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    m_F->glEnableVertexAttribArray(0);

    m_F->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    m_F->glEnableVertexAttribArray(1);
}

void GlManager::Axis::Destroy()
{
    m_F->glDeleteProgram(m_AxisEndShaderProgramID);
    m_F->glDeleteProgram(m_AxisLineShaderProgramID);

    m_F->glDeleteVertexArrays(1, &m_AxisEndVAOID);
    m_F->glDeleteVertexArrays(1, &m_AxisLineVAOID);
    m_F->glDeleteBuffers(1, &m_AxisEndVBOID);
    m_F->glDeleteBuffers(1, &m_AxisLineVBOID);
}

void GlManager::Axis::Render(double Length, double Size, glm::mat4* MVP)
{
    m_F->glDisable(GL_DEPTH_TEST);

    m_F->glUseProgram(m_AxisEndShaderProgramID);

    auto SomeAxis = [&](glm::vec3 Which)
    {
        glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(Size));
        glm::mat4 T = glm::translate(glm::mat4(1), Which * (float)Length);

        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisEndShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0] * T * S));
        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisEndShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisEndShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));
        m_F->glUniform3f(m_F->glGetUniformLocation(m_AxisEndShaderProgramID, "axisColor"), Which.x, Which.y, Which.z);

        m_F->glBindVertexArray(m_AxisEndVAOID);
        m_F->glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    SomeAxis(glm::vec3(1, 0, 0));
    SomeAxis(glm::vec3(0, 1, 0));
    SomeAxis(glm::vec3(0, 0, 1));

    m_F->glUseProgram(m_AxisLineShaderProgramID);

    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisLineShaderProgramID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisLineShaderProgramID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_AxisLineShaderProgramID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));
    m_F->glUniform1f(m_F->glGetUniformLocation(m_AxisLineShaderProgramID, "lineLength"), Length);

    m_F->glLineWidth(3);

    m_F->glBindVertexArray(m_AxisLineVAOID);
    m_F->glDrawArrays(GL_LINES, 0, 6);

    m_F->glEnable(GL_DEPTH_TEST);
}

void GlManager::Outline::Init()
{
    uint32_t outlineVertShaderID = GlMisc::CompileShader(m_F, Shader("GLOutline", sType::VERT).m_ShaderCode.data(), sType::VERT);
    uint32_t outlineFragShaderID = GlMisc::CompileShader(m_F, Shader("GLOutline", sType::FRAG).m_ShaderCode.data(), sType::FRAG);

    m_OutlineShaderID = m_F->glCreateProgram();
    m_F->glAttachShader(m_OutlineShaderID, outlineVertShaderID);
    m_F->glAttachShader(m_OutlineShaderID, outlineFragShaderID);
    m_F->glLinkProgram (m_OutlineShaderID);

    m_F->glDeleteShader(outlineVertShaderID);
    m_F->glDeleteShader(outlineFragShaderID);
}

void GlManager::Outline::Destroy()
{
    m_F->glDeleteProgram(m_OutlineShaderID);
}

void GlManager::ShaderManager::Init()
{
    // HARD CODE
    m_SupportShaders.push_back("Default");
    m_SupportShadersPath.push_back("GLDefault");
    m_RevSupportShaders[std::string("Default")] = 0;

    m_SupportShaders.push_back("Glass");
    m_SupportShadersPath.push_back("GLGlass");
    m_RevSupportShaders[std::string("Glass")] = 1;

    m_SupportShaders.push_back("Metal");
    m_SupportShadersPath.push_back("GLMetal");
    m_RevSupportShaders[std::string("Metal")] = 2;


    m_SupportShaders.push_back("Cartoon");
    m_SupportShadersPath.push_back("GLCartoon");
    m_RevSupportShaders[std::string("Cartoon")] = 3;

    m_SupportShadersChar = "Default\0Glass\0Metal\0Cartoon\0";
    // HARD CODE

    auto loadAGroupShader = [](QOpenGLFunctions_3_3_Core* m_F, const std::string& ShaderPath)->uint32_t
    {
        uint32_t vertShader = GlMisc::CompileShader(m_F, Shader(ShaderPath.c_str(), sType::VERT).m_ShaderCode.data(), sType::VERT);
        uint32_t fragShader = GlMisc::CompileShader(m_F, Shader(ShaderPath.c_str(), sType::FRAG).m_ShaderCode.data(), sType::FRAG);

        uint32_t ShaderProgramID = m_F->glCreateProgram();
        m_F->glAttachShader(ShaderProgramID, vertShader);
        m_F->glAttachShader(ShaderProgramID, fragShader);
        m_F->glLinkProgram (ShaderProgramID);

        m_F->glDeleteShader(vertShader);
        m_F->glDeleteShader(fragShader);

        return ShaderProgramID;
    };
    
    // HARD CODE
    m_DefaultShaderProgramID = loadAGroupShader(m_F, m_SupportShadersPath[0]);
    m_GlassShaderProgramID   = loadAGroupShader(m_F, m_SupportShadersPath[1]);
    m_MetalShaderProgramID   = loadAGroupShader(m_F, m_SupportShadersPath[2]);
    m_CartoonShaderProgramID = loadAGroupShader(m_F, m_SupportShadersPath[3]);
    // HARD CODE
}

void GlManager::ShaderManager::Destroy()
{
    m_F->glDeleteProgram(m_DefaultShaderProgramID);
    m_F->glDeleteProgram(m_GlassShaderProgramID);
    m_F->glDeleteProgram(m_MetalShaderProgramID);
    m_F->glDeleteProgram(m_CartoonShaderProgramID);
}

bool GlManager::Init()
{
    m_F->glGenFramebuffers(1, &m_SceneFBO);
    m_F->glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    m_F->glGenTextures(1, &m_SceneTextureID);
    m_F->glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);

    m_F->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_F->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_F->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    
    m_F->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTextureID, 0);
    m_F->glBindTexture(GL_TEXTURE_2D, 0);

    m_F->glGenTextures(1, &m_DepthTextureID);
    m_F->glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    
    m_F->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 128, 128, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    
    m_F->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
    m_F->glBindTexture(GL_TEXTURE_2D, 0);

    if (GL_FRAMEBUFFER_COMPLETE != m_F->glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        Out::Log(pType::CRASH, "Init FBO Failed");
        return false;
    }

    m_F->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_Skybox.Init();
    m_Grid.Init();
    m_Axis.Init();
    m_Outline.Init();
    m_ShaderManager.Init();

    Out::Log(pType::MESSAGE, "Inited OpenGL");

    return true;
}

bool GlManager::Destroy()
{
    m_F->glDeleteFramebuffers(1, &m_SceneFBO);

    m_F->glDeleteTextures(1, &m_SceneTextureID);
    m_F->glDeleteTextures(1, &m_DepthTextureID);

    m_Skybox.Destroy();
    m_Grid.Destroy();
    m_Axis.Destroy();
    m_Outline.Destroy();
    m_ShaderManager.Destroy();

    Out::Log(pType::MESSAGE, "Cleaned OpenGL");

    return true;
}

void GlManager::BeginRenderEditor(uint32_t viewW, uint32_t viewH)
{
    m_ViewWidth  = viewW;
    m_ViewHeight = viewH;
    m_CameraPos  = mainCamera->getPosition();
    m_CameraDir  = mainCamera->getForward();

    m_P = glm::perspective(glm::radians(mainCamera->getFOV()), (double)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    m_F->glBindFramebuffer(GL_FRAMEBUFFER, m_SceneFBO);

    m_F->glEnable(GL_DEPTH_TEST);

    m_F->glBindTexture(GL_TEXTURE_2D, m_SceneTextureID);
    m_F->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, viewW, viewH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    m_F->glBindTexture(GL_TEXTURE_2D, 0);

    m_F->glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    m_F->glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewW, viewH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    m_F->glBindTexture(GL_TEXTURE_2D, 0);

    m_F->glViewport(0, 0, viewW, viewH);

    m_F->glClearColor(colorGray.x(), colorGray.y(), colorGray.z(), 1.0);
    m_F->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlManager::EndRenderEditor()
{
    m_F->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlManager::BeginRenderGame(uint32_t viewW, uint32_t viewH)
{
    
    m_ViewWidth  = viewW;
    m_ViewHeight = viewH;
    m_CameraPos  = mainCamera->getPosition();
    m_CameraDir  = mainCamera->getForward();

    m_P = glm::perspective(glm::radians(mainCamera->getFOV()), (double)m_ViewWidth / m_ViewHeight, NearZ, FarZ);

    // Not default 0 in Qt
    // m_F->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_F->glEnable(GL_DEPTH_TEST);

    m_F->glViewport(0, 0, viewW, viewH);

    m_F->glClearColor(colorGreen.x(), colorGreen.y(), colorGreen.z(), 1.0);
    m_F->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlManager::EndRenderGame()
{
    // End Render Game
}

void GlManager::Render
(
    GlRenderable* renderObj,
    const Transform& ObjTransform, bool Selected, bool LineMode
)
{
    if (!renderObj) return;

    glm::mat4 MVP[3];

    glm::mat4 ModelWithoutScale;

    // Transform
    MVP[0] = glm::mat4(1);
    MVP[0] = glm::translate(MVP[0], Convert(ObjTransform.Location));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.y()), glm::vec3(0, 1, 0));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.x()), glm::vec3(1, 0, 0));
    MVP[0] = glm::rotate(MVP[0], glm::radians((float)ObjTransform.Rotation.z()), glm::vec3(0, 0, 1));

    ModelWithoutScale = MVP[0];

    MVP[0] = glm::scale(MVP[0], Convert(ObjTransform.Scale));

    mainCamera->updateView(MVP[1]);

    MVP[2] = m_P;

    if (LineMode) m_F->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), m_F->glLineWidth(1);

    unsigned int usedShaderID = 0;

    if (renderObj->m_Shader == "Default")
    {
        m_F->glUseProgram(m_ShaderManager.m_DefaultShaderProgramID);
        usedShaderID = m_ShaderManager.m_DefaultShaderProgramID;

        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "lightDir"), m_LightDir.x(), m_LightDir.y(), m_LightDir.z());
        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "lightColor"), m_LightColor.x(), m_LightColor.y(), m_LightColor.z());
        m_F->glUniform1f(m_F->glGetUniformLocation(usedShaderID, "lightPower"), m_LightPower);

        auto cameraPos = mainCamera->getPosition();
        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "viewLocation"), cameraPos.x(), cameraPos.y(), cameraPos.z());

        auto pColor = &renderObj->m_DiffuseColor;
        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "diffuseColor"), pColor->x(), pColor->y(), pColor->z());

        if (renderObj->m_EnableDiffuseTexture)
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableDiffuseTexture"), 1);
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "diffuseTexture"), 0);

            m_F->glActiveTexture(GL_TEXTURE0);
            m_F->glBindTexture(GL_TEXTURE_2D, renderObj->m_DiffuseTextureID);
        }
        else
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableDiffuseTexture"), 0);
        }

        if (renderObj->m_EnableNormalTexture)
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableNormalTexture"), 1);
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "normalTexture"), 1);

            m_F->glActiveTexture(GL_TEXTURE1);
            m_F->glBindTexture(GL_TEXTURE_2D, renderObj->m_NormalTextureID);
        }
        else
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableNormalTexture"), 0);
        }

        if (renderObj->m_EnableSpecularTexture)
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableSpecularTexture"), 1);
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "specularTexture"), 2);

            m_F->glActiveTexture(GL_TEXTURE2);
            m_F->glBindTexture(GL_TEXTURE_2D, renderObj->m_SpecularTextureID);
        }
        else
        {
            m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "enableSpecularTexture"), 0);
        }
    }
    else if (renderObj->m_Shader == "Glass")
    {
        m_F->glUseProgram(m_ShaderManager.m_GlassShaderProgramID);
        usedShaderID = m_ShaderManager.m_GlassShaderProgramID;

        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "cameraPos"),
            m_CameraPos.x(), m_CameraPos.y(), m_CameraPos.z());
        m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "skyboxTexture"), 0);
        m_F->glUniform1f(m_F->glGetUniformLocation(usedShaderID, "N"), renderObj->m_N);

        m_F->glActiveTexture(GL_TEXTURE0);
        m_F->glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.m_SkyboxTextureID);
    }
    else if (renderObj->m_Shader == "Metal")
    {
        m_F->glUseProgram(m_ShaderManager.m_MetalShaderProgramID);
        usedShaderID = m_ShaderManager.m_MetalShaderProgramID;

        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "cameraPos"),
            m_CameraPos.x(), m_CameraPos.y(), m_CameraPos.z());
        m_F->glUniform1i(m_F->glGetUniformLocation(usedShaderID, "skyboxTexture"), 0);

        m_F->glActiveTexture(GL_TEXTURE0);
        m_F->glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.m_SkyboxTextureID);
    }
    else if (renderObj->m_Shader == "Cartoon")
    {
        m_F->glUseProgram(m_ShaderManager.m_CartoonShaderProgramID);
        usedShaderID = m_ShaderManager.m_CartoonShaderProgramID;

        auto pColor = &renderObj->m_DiffuseColor;
        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "diffuseColor"), pColor->x(), pColor->y(), pColor->z());

        m_F->glUniform3f(m_F->glGetUniformLocation(usedShaderID, "lightDir"), m_LightDir.x(), m_LightDir.y(), m_LightDir.z());
    }
    else
    {
        return;
    }

    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(usedShaderID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(usedShaderID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
    m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(usedShaderID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));

    if (Selected)
    {
        m_F->glEnable(GL_STENCIL_TEST);

        m_F->glStencilMask(0xFF);
        m_F->glClear(GL_STENCIL_BUFFER_BIT);

        m_F->glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        m_F->glStencilFunc(GL_ALWAYS, 1, 0xFF);
    }

    renderObj->Draw();

    if (Selected)
    {
        m_F->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        m_F->glStencilMask(0x00);
        m_F->glDisable(GL_DEPTH_TEST);

        m_F->glUseProgram(m_Outline.m_OutlineShaderID);
        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_Outline.m_OutlineShaderID, "M"), 1, GL_FALSE, glm::value_ptr(MVP[0]));
        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_Outline.m_OutlineShaderID, "V"), 1, GL_FALSE, glm::value_ptr(MVP[1]));
        m_F->glUniformMatrix4fv(m_F->glGetUniformLocation(m_Outline.m_OutlineShaderID, "P"), 1, GL_FALSE, glm::value_ptr(MVP[2]));

        renderObj->Draw();

        m_F->glDisable(GL_STENCIL_TEST);
        m_F->glEnable(GL_DEPTH_TEST);
    }

    if (LineMode) m_F->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (Selected)
    {
        MVP[0] = ModelWithoutScale;
        m_Axis.Render(1, 0.1, MVP);
    }
}

GlView::GlView(QWidget *parent) : QOpenGLWidget(parent), Manager(this, &Camera)
{
    QObject::connect(&TickTimer, &QTimer::timeout, this, &GlView::Tick);
    TickTimer.start(50);
}

void GlView::Destroy()
{
    for(auto i=m_NameToObjectPtr.begin();i!=m_NameToObjectPtr.end();++i)
    {        
        std::shared_ptr<SceneObject> ThisObj = i->second;
        ThisObj->Renderable.Clear();
    }

    Manager.Destroy();
}

void GlView::initializeGL()
{
    initializeOpenGLFunctions();

    Manager.Init();

    Camera.Init();
} 

void GlView::paintGL()
{
    if(!MeshToLoad.empty())
    {
        auto Mesh = MeshToLoad.front();
        MeshToLoad.pop();

        auto FilePath = Mesh.second;

        auto ThisObj = std::make_shared<SceneObject>();

        ThisObj->Name = Mesh.first;
        m_NameToObjectPtr[Mesh.first] = ThisObj;

        if(!m_NameToMaterialPtr.empty())
        {
            ThisObj->MaterialName = m_NameToMaterialPtr.begin()->first;
        }

        ThisObj->Renderable.Init(this);

        ThisObj->Renderable.setShader("Default");
        ThisObj->Renderable.reLoadModel(FilePath);
        ThisObj->Renderable.setDiffuseColor(colorGray);
    }

    if(!SphereToLoad.empty())
    {
        auto SphereName = SphereToLoad.front();
        SphereToLoad.pop();

        auto ThisObj = std::make_shared<SceneObject>();

        ThisObj->Name = SphereName;
        m_NameToObjectPtr[SphereName] = ThisObj;

        if(!m_NameToMaterialPtr.empty())
        {
            ThisObj->MaterialName = m_NameToMaterialPtr.begin()->first;
        }

        ThisObj->Renderable.Init(this);

        ThisObj->Renderable.setShader("Default");
        ThisObj->Renderable.reLoadSphere();
        ThisObj->Renderable.setDiffuseColor(colorGray);

        ThisObj->Renderable.isTriangle = false;
    }

    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto NowWidth = width();
    auto NowHeight = height();

    Manager.BeginRenderGame(NowWidth, NowHeight);

    Manager.RenderSkybox();
    Manager.RenderGrid();

    for(auto i=m_NameToObjectPtr.begin();i!=m_NameToObjectPtr.end();++i)
    {        
        std::shared_ptr<SceneObject> ThisObj = i->second;

        bool Selected = (SelectedObjectName == ThisObj->Name);
        
        auto P_Renderable = &(ThisObj->Renderable);
        auto MatName = ThisObj->MaterialName;
        if(m_NameToMaterialPtr.count(MatName))
        {
            if(m_NameToMaterialPtr[MatName]->GetType() == GMType::DIFFUSE)
            {
                GlDiffuse* Mat = (GlDiffuse*)(m_NameToMaterialPtr[MatName].get());
                P_Renderable->setShader("Default");
                P_Renderable->setDiffuseColor(Mat->BaseColor);
            }
            else if(m_NameToMaterialPtr[MatName]->GetType() == GMType::METAL)
            {
                GlMetal* Mat = (GlMetal*)(m_NameToMaterialPtr[MatName].get());
                P_Renderable->setShader("Default");
                P_Renderable->setDiffuseColor(Mat->BaseColor);
            }
            else if(m_NameToMaterialPtr[MatName]->GetType() == GMType::GLASS)
            {
                GlGlass* Mat = (GlGlass*)(m_NameToMaterialPtr[MatName].get());
                P_Renderable->setN(1.2);
                P_Renderable->setShader("Glass");
            }
            else if(m_NameToMaterialPtr[MatName]->GetType() == GMType::EMIT)
            {
                GlEmit* Mat = (GlEmit*)(m_NameToMaterialPtr[MatName].get());
                P_Renderable->setShader("Default");
                P_Renderable->setDiffuseColor(Mat->BaseColor);
            }
            else if(m_NameToMaterialPtr[MatName]->GetType() == GMType::FOG)
            {
                GlFog* Mat = (GlFog*)(m_NameToMaterialPtr[MatName].get());
                P_Renderable->setShader("Default");
                P_Renderable->setDiffuseColor(colorGray);
            }
        }

        Manager.Render(P_Renderable, ThisObj->Trans, Selected, LineMode);
    }

    Manager.EndRenderGame();
}

void GlView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & (Qt::LeftButton | Qt::RightButton))
    {
        Camera.Cursor(event->x(), event->y(), true, false);
    }
}

void GlView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & (Qt::LeftButton | Qt::RightButton))
    {
        Camera.Cursor(event->x(), event->y(), false, false);
        update();
    }
}

void GlView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        if(event->isAutoRepeat()) return;
        KW = true;
    }else if(event->key() == Qt::Key_S)
    {
        if(event->isAutoRepeat()) return;
        KS = true;
    }

    if(event->key() == Qt::Key_A)
    {
        if(event->isAutoRepeat()) return;
        KA = true;
    }else if(event->key() == Qt::Key_D)
    {
        if(event->isAutoRepeat()) return;
        KD = true;
    }
}

void GlView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        if(event->isAutoRepeat()) return;
        KW = false;
    }else if(event->key() == Qt::Key_S)
    {
        if(event->isAutoRepeat()) return;
        KS = false;
    }

    if(event->key() == Qt::Key_A)
    {
        if(event->isAutoRepeat()) return;
        KA = false;
    }else if(event->key() == Qt::Key_D)
    {
        if(event->isAutoRepeat()) return;
        KD = false;
    }
}

void GlView::Tick()
{
    const double cameraMoveSpeed = 20;
    const double deltaTime = 0.05;

    Direction cameraForward = Camera.getForward();
    Direction cameraRight   = Camera.getRight();

    bool HasEvent = true;

    if(KW)
    {
        Camera.Move(cameraForward * deltaTime * cameraMoveSpeed);
    }else if(KS)
    {
        Camera.Move(cameraForward * deltaTime * cameraMoveSpeed * -1);
    }else if(KD)
    {
        Camera.Move(cameraRight * deltaTime * cameraMoveSpeed);
    }else if(KA)
    {
        Camera.Move(cameraRight * deltaTime * cameraMoveSpeed * -1);
    }else
    {
        HasEvent = false;
    }

    if(HasEvent) update();
}

void GlView::LoadMesh(QString BaseName, QString FilePath)
{
    MeshToLoad.push(std::make_pair(BaseName.toStdString(), FilePath.toStdString()));

    update();
}

void GlView::LoadSphere(QString BaseName)
{
    SphereToLoad.push(BaseName.toStdString());

    update();
}

void GlView::UpdateTransform(const Transform& NewTransform)
{
    if(m_NameToObjectPtr.count(SelectedObjectName))
    {
        auto Obj = m_NameToObjectPtr[SelectedObjectName];
        Obj->Trans = NewTransform;

        if(!Obj->Renderable.isTriangle)
        {
            Obj->Trans.Scale[1] = Obj->Trans.Scale[0];
            Obj->Trans.Scale[2] = Obj->Trans.Scale[0];
        }

        update();
    }
}

Transform GlView::GetTransform()
{
    if(m_NameToObjectPtr.count(SelectedObjectName))
    {
        return m_NameToObjectPtr[SelectedObjectName]->Trans;
    }

    return Transform();
}

void GlView::AttachMaterial(const std::string& MaterialName)
{
    if(m_NameToObjectPtr.count(SelectedObjectName))
    {
        auto Obj = m_NameToObjectPtr[SelectedObjectName];
        Obj->MaterialName = MaterialName;

        update();
    }
}

std::string GlView::GetMaterial()
{
    if(m_NameToObjectPtr.count(SelectedObjectName))
    {
        return m_NameToObjectPtr[SelectedObjectName]->MaterialName;
    }

    return "";
}

void GlView::AddMaterial(const std::string& MaterialName)
{
    m_NameToMaterialPtr[MaterialName] = std::make_shared<GlDiffuse>();
    m_NameToMaterialPtr[MaterialName]->Name = MaterialName;
}

void GlView::PackageScene(PSceneRequest& SceneData)
{
    auto Location = Camera.getPosition();
    auto Forward = Camera.getForward();
    auto Right = Camera.getRight();

    Vec3* CameraLocation = new Vec3;
    CameraLocation->set_x(Location.x());
    CameraLocation->set_y(Location.y());
    CameraLocation->set_z(Location.z());
    SceneData.set_allocated_cameralocation(CameraLocation);

    Vec3* CameraForward = new Vec3;
    CameraForward->set_x(Forward.x());
    CameraForward->set_y(Forward.y());
    CameraForward->set_z(Forward.z());
    SceneData.set_allocated_cameraforward(CameraForward);

    Vec3* CameraRight = new Vec3;
    CameraRight->set_x(Right.x());
    CameraRight->set_y(Right.y());
    CameraRight->set_z(Right.z());
    SceneData.set_allocated_cameraright(CameraRight);

    for(auto i=m_NameToMaterialPtr.begin();i!=m_NameToMaterialPtr.end();++i)
    {
        auto Material = SceneData.add_materials();

        std::shared_ptr<GlMaterial> ThisMat = i->second;

        Material->set_name(i->first);

        if(ThisMat->GetType() == GMType::DIFFUSE)
        {
            Material->set_type(1);

            GlDiffuse* Diffuse = (GlDiffuse*)(ThisMat.get());
            Material->add_parameters(Diffuse->BaseColor.x());
            Material->add_parameters(Diffuse->BaseColor.y());
            Material->add_parameters(Diffuse->BaseColor.z());

        }else if(ThisMat->GetType() == GMType::METAL)
        {
            Material->set_type(2);

            GlMetal* Metal = (GlMetal*)(ThisMat.get());
            Material->add_parameters(Metal->BaseColor.x());
            Material->add_parameters(Metal->BaseColor.y());
            Material->add_parameters(Metal->BaseColor.z());
            Material->add_parameters(Metal->Fuzzy);

        }else if(ThisMat->GetType() == GMType::GLASS)
        {
            Material->set_type(3);

            GlGlass* Glass = (GlGlass*)(ThisMat.get());
            Material->add_parameters(Glass->IOR);

        }else if(ThisMat->GetType() == GMType::EMIT)
        {
            Material->set_type(4);

            GlEmit* Emit = (GlEmit*)(ThisMat.get());
            Material->add_parameters(Emit->BaseColor.x());
            Material->add_parameters(Emit->BaseColor.y());
            Material->add_parameters(Emit->BaseColor.z());
            Material->add_parameters(Emit->Intensity);
        }else if(ThisMat->GetType() == GMType::FOG)
        {
            Material->set_type(5);

            GlFog* Fog = (GlFog*)(ThisMat.get());
        }
    }

    for(auto i=m_NameToObjectPtr.begin();i!=m_NameToObjectPtr.end();++i)
    {
        std::shared_ptr<SceneObject> ThisObj = i->second;

        auto ObjTransform = ThisObj->Trans;

        if(ThisObj->Renderable.isTriangle)
        {
            auto Mesh = SceneData.add_meshs();

            auto ModelMatrix = glm::mat4(1);
            ModelMatrix = glm::translate(ModelMatrix, Convert(ObjTransform.Location));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)ObjTransform.Rotation.y()), glm::vec3(0, 1, 0));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)ObjTransform.Rotation.x()), glm::vec3(1, 0, 0));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)ObjTransform.Rotation.z()), glm::vec3(0, 0, 1));
            ModelMatrix = glm::scale(ModelMatrix, Convert(ObjTransform.Scale));
            
            const GlRenderable* ThisRenderable = &(ThisObj->Renderable);

            Mesh->set_matname(ThisObj->MaterialName);

            // gl_Position = P * V * M * vec4(aPos, 1.0);
            for(int j=0;j<ThisRenderable->m_Triangles.size();j+=1)
            {
                auto ThisTriangle = Mesh->add_triangles();
                // *ThisTriangle = ThisRenderable->m_Triangles[j];

                auto LocalTriangle = ThisRenderable->m_Triangles[j];

                {
                    Vec3 LocalPoint = LocalTriangle.a();

                    glm::vec4 aPos(LocalPoint.x(), LocalPoint.y(), LocalPoint.z(), 1.0f);
                    auto TransedPoint = ModelMatrix * aPos;

                    Vec3* TransedTriPoint = new Vec3;
                    TransedTriPoint->set_x(TransedPoint[0] / TransedPoint[3]);
                    TransedTriPoint->set_y(TransedPoint[1] / TransedPoint[3]);
                    TransedTriPoint->set_z(TransedPoint[2] / TransedPoint[3]);

                    ThisTriangle->set_allocated_a(TransedTriPoint);
                }

                {
                    Vec3 LocalPoint = LocalTriangle.b();

                    glm::vec4 aPos(LocalPoint.x(), LocalPoint.y(), LocalPoint.z(), 1.0f);
                    auto TransedPoint = ModelMatrix * aPos;

                    Vec3* TransedTriPoint = new Vec3;
                    TransedTriPoint->set_x(TransedPoint[0] / TransedPoint[3]);
                    TransedTriPoint->set_y(TransedPoint[1] / TransedPoint[3]);
                    TransedTriPoint->set_z(TransedPoint[2] / TransedPoint[3]);

                    ThisTriangle->set_allocated_b(TransedTriPoint);
                }

                {
                    Vec3 LocalPoint = LocalTriangle.c();

                    glm::vec4 aPos(LocalPoint.x(), LocalPoint.y(), LocalPoint.z(), 1.0f);
                    auto TransedPoint = ModelMatrix * aPos;

                    Vec3* TransedTriPoint = new Vec3;
                    TransedTriPoint->set_x(TransedPoint[0] / TransedPoint[3]);
                    TransedTriPoint->set_y(TransedPoint[1] / TransedPoint[3]);
                    TransedTriPoint->set_z(TransedPoint[2] / TransedPoint[3]);

                    ThisTriangle->set_allocated_c(TransedTriPoint);
                }
                
            }
        }else
        {
            auto ThisSphere = SceneData.add_spheres();

            ThisSphere->set_matname(ThisObj->MaterialName);

            Vec3* Location = new Vec3;
            Location->set_x(ObjTransform.Location[0]);
            Location->set_y(ObjTransform.Location[1]);
            Location->set_z(ObjTransform.Location[2]);

            ThisSphere->set_allocated_center(Location);
            ThisSphere->set_radious(ObjTransform.Scale[0]);
        }
        
    }    
}