#include "GlRenderable.h"

#define G_IMPORT_PATH "" // from reEngine, but removed here

void GlRenderable::reLoadModel(const std::string& Model)
{
    if (Model == m_NowModel) return;

    // Location UV : 14 Float per Vertex
    std::vector<float> VBuffer;
    std::vector<unsigned int> EBuffer;

    m_Triangles.clear();

    // Load Model
    Loader::ModelLoader modelLoader;
    if (modelLoader.Load((G_IMPORT_PATH + Model).c_str()))
    {
        for (auto meshIndex = modelLoader.m_Meshs.begin(); meshIndex != modelLoader.m_Meshs.end(); ++meshIndex)
        {
            int VertexNumber = VBuffer.size() / 14;
            for (auto faceIndex = (*meshIndex)->Faces.begin(); faceIndex != (*meshIndex)->Faces.end(); ++faceIndex)
            {
                EBuffer.push_back((*faceIndex)->x() + VertexNumber);
                EBuffer.push_back((*faceIndex)->y() + VertexNumber);
                EBuffer.push_back((*faceIndex)->z() + VertexNumber);
            }

            auto pV = &(*meshIndex)->Vertices;
            for (auto vertexIndex = pV->begin(); vertexIndex != pV->end(); ++vertexIndex)
            {
                VBuffer.push_back((*vertexIndex)->m_Location.x());
                VBuffer.push_back((*vertexIndex)->m_Location.y());
                VBuffer.push_back((*vertexIndex)->m_Location.z());
                VBuffer.push_back((*vertexIndex)->m_Normal.x());
                VBuffer.push_back((*vertexIndex)->m_Normal.y());
                VBuffer.push_back((*vertexIndex)->m_Normal.z());
                VBuffer.push_back((*vertexIndex)->m_TextureCoords.x());
                VBuffer.push_back((*vertexIndex)->m_TextureCoords.y());
                VBuffer.push_back((*vertexIndex)->m_Tangent.x());
                VBuffer.push_back((*vertexIndex)->m_Tangent.y());
                VBuffer.push_back((*vertexIndex)->m_Tangent.z());
                VBuffer.push_back((*vertexIndex)->m_Bitangent.x());
                VBuffer.push_back((*vertexIndex)->m_Bitangent.y());
                VBuffer.push_back((*vertexIndex)->m_Bitangent.z());
            }
        }

        // std::vector<float> VBuffer;
        // std::vector<unsigned int> EBuffer;

        // Danger
        int FaceCount = EBuffer.size() / 3;
        for(int i=0;i<FaceCount;i+=1)
        {
            int A_Index = EBuffer[i*3 + 0];
            int B_Index = EBuffer[i*3 + 1];
            int C_Index = EBuffer[i*3 + 2];

            PSceneRequest_Triangle ThisTriangle;

            Vec3* A = new Vec3;
            A->set_x(VBuffer[A_Index * 14 + 0]);
            A->set_y(VBuffer[A_Index * 14 + 1]);
            A->set_z(VBuffer[A_Index * 14 + 2]);
            ThisTriangle.set_allocated_a(A);

            Vec3* B = new Vec3;
            B->set_x(VBuffer[B_Index * 14 + 0]);
            B->set_y(VBuffer[B_Index * 14 + 1]);
            B->set_z(VBuffer[B_Index * 14 + 2]);
            ThisTriangle.set_allocated_b(B);

            Vec3* C = new Vec3;
            C->set_x(VBuffer[C_Index * 14 + 0]);
            C->set_y(VBuffer[C_Index * 14 + 1]);
            C->set_z(VBuffer[C_Index * 14 + 2]);
            ThisTriangle.set_allocated_c(C);

            m_Triangles.push_back(ThisTriangle);
        }
    }
    else
    {
        Out::Log(pType::CRASH, "Load Model File Failed");
        return;
    }

    if (m_VAOID) m_F->glDeleteVertexArrays(1, &m_VAOID);
    if (m_VBOID) m_F->glDeleteBuffers(1, &m_VBOID);
    if (m_EBOID) m_F->glDeleteBuffers(1, &m_EBOID);

    m_ElementCount = EBuffer.size();

    m_F->glGenVertexArrays(1, &m_VAOID);
    m_F->glGenBuffers(1, &m_VBOID);
    m_F->glGenBuffers(1, &m_EBOID);

    m_F->glBindVertexArray(m_VAOID);
    m_F->glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
    m_F->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VBuffer.size(), VBuffer.data(), GL_STATIC_DRAW);

    m_F->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOID);
    m_F->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * EBuffer.size(), EBuffer.data(), GL_STATIC_DRAW);

    m_F->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    m_F->glEnableVertexAttribArray(0);

    m_F->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    m_F->glEnableVertexAttribArray(1);

    m_F->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    m_F->glEnableVertexAttribArray(2);

    m_F->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    m_F->glEnableVertexAttribArray(3);

    m_F->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    m_F->glEnableVertexAttribArray(4);

    m_NowModel = Model;
}

void GlRenderable::reLoadDiffuseTexture(const std::string& DiffuseTextureName)
{
    if (m_NowDiffuseTexture == DiffuseTextureName) return;

    if (m_DiffuseTextureID)  m_F->glDeleteTextures(1, &m_DiffuseTextureID);
    m_DiffuseTextureID = GlMisc::GenDefaultTextureWithImageFile(m_F, (G_IMPORT_PATH + DiffuseTextureName).c_str());

    m_NowDiffuseTexture = DiffuseTextureName;
}

void GlRenderable::reLoadNormalTexture(const std::string& NormalTextureName)
{
    if (m_NowNormalTexture == NormalTextureName) return;

    if (m_NormalTextureID)   m_F->glDeleteTextures(1, &m_NormalTextureID);
    m_NormalTextureID = GlMisc::GenDefaultTextureWithImageFile(m_F, (G_IMPORT_PATH + NormalTextureName).c_str());

    m_NowNormalTexture = NormalTextureName;
}

void GlRenderable::reLoadSpecularTexture(const std::string& SpecularTextureName)
{
    if (m_NowSpecularTexture == SpecularTextureName) return;

    if (m_SpecularTextureID) m_F->glDeleteTextures(1, &m_SpecularTextureID);
    m_SpecularTextureID = GlMisc::GenDefaultTextureWithImageFile(m_F, (G_IMPORT_PATH + SpecularTextureName).c_str());

    m_NowSpecularTexture = SpecularTextureName;
}

void GlRenderable::reLoadSphere()
{
    reLoadModel("./Asset/Sphere/Sphere.obj");
}

void GlRenderable::Clear()
{
    if (m_VAOID) m_F->glDeleteVertexArrays(1, &m_VAOID);
    if (m_VBOID) m_F->glDeleteBuffers(1, &m_VBOID);
    if (m_EBOID) m_F->glDeleteBuffers(1, &m_EBOID);

    if (m_DiffuseTextureID)  m_F->glDeleteTextures(1, &m_DiffuseTextureID);
    if (m_NormalTextureID)   m_F->glDeleteTextures(1, &m_NormalTextureID);
    if (m_SpecularTextureID) m_F->glDeleteTextures(1, &m_SpecularTextureID);

    if(m_DirectShaderProgramID) m_F->glDeleteShader(m_DirectShaderProgramID);
}

void GlRenderable::Draw()
{
    if (!m_VAOID) return;

    m_F->glBindVertexArray(m_VAOID);
    m_F->glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_INT, 0);
}