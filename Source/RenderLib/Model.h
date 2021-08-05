#pragma once

#include<vector>
#include<memory>
#include<string>
#include<cstdint>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"Material.h"
#include"MathAllInOne.h"

class Vertex
{
public:
	Vertex() = default;
	~Vertex() = default;

public:
	Point m_Location;
	Direction m_Normal;
	TextureCoords m_TextureCoords;
};

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;

public:
	friend class Model;
	friend class Scene;

protected:
	std::vector<std::shared_ptr<Vertex>> Vertices;
	std::vector<std::shared_ptr<TriangleFace>> Faces;

	std::shared_ptr<Material> m_Material;
};

class Model
{
public:
	Model()
	{
		Loader = std::make_unique<Assimp::Importer>();
	}
	~Model() = default;

public:
	friend class Scene;

protected:
	std::unique_ptr<Assimp::Importer> Loader;
	std::string FileDir;

	std::vector<std::shared_ptr<Mesh>> m_Meshs;

public:
	bool Load(const char* filePath)
	{
		const aiScene* scene = Loader->ReadFile(filePath, aiProcess_Triangulate);

		//Error
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return false;
		}

		//Only Work On Windows!
		FileDir = std::string(filePath);
		FileDir = FileDir.substr(0, FileDir.find_last_of('\\'));

		ProcessNode(scene->mRootNode, scene);

		return true;
	}

private:
	void ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		auto ThisMesh = std::make_shared<Mesh>();

		//Mesh Name

		for (uint32_t i = 0; i < mesh->mNumVertices; i += 1)
		{
			auto ThisVertex = std::make_shared<Vertex>();

			ThisVertex->m_Location.x() = mesh->mVertices[i].x;
			ThisVertex->m_Location.y() = mesh->mVertices[i].y;
			ThisVertex->m_Location.z() = mesh->mVertices[i].z;

			ThisVertex->m_Normal.x() = mesh->mNormals[i].x;
			ThisVertex->m_Normal.y() = mesh->mNormals[i].y;
			ThisVertex->m_Normal.z() = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{
				ThisVertex->m_TextureCoords.x() = mesh->mTextureCoords[0][i].x;
				ThisVertex->m_TextureCoords.y() = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				ThisVertex->m_TextureCoords.x() = 0;
				ThisVertex->m_TextureCoords.y() = 0;
			}

			ThisMesh->Vertices.push_back(ThisVertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i += 1)
		{
			if (mesh->mFaces[i].mNumIndices != 3) continue;

			auto ThisFace = std::make_shared<TriangleFace>();
			ThisFace->x() = mesh->mFaces[i].mIndices[0];
			ThisFace->y() = mesh->mFaces[i].mIndices[1];
			ThisFace->z() = mesh->mFaces[i].mIndices[2];

			ThisMesh->Faces.push_back(ThisFace);
		}

		//Material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
			//Try to Save Material Here

			//Plane
			if (meshMaterial->GetName() == aiString("Plane"))
			{
				auto materialDiff = std::make_shared<Diffuse>();
				materialDiff->baseColor = colorGray;

				ThisMesh->m_Material = materialDiff;
			}
			else
			{
				switch (RandomIntger(1, 4))
				{
				case 1:
				{
					auto thisMaterial = std::make_shared<Diffuse>();
					thisMaterial->baseColor = Color(UniformRandom(), UniformRandom(), UniformRandom());

					ThisMesh->m_Material = thisMaterial;
				}
				break;

				case 2:
				{
					auto thisMaterial = std::make_shared<Glass>();
					thisMaterial->baseColor = Color(UniformRandom(), UniformRandom(), UniformRandom());
					thisMaterial->Roughness = 0;
					thisMaterial->IOR = UniformRandom(1.2, 2);

					ThisMesh->m_Material = thisMaterial;
				}
				break;

				case 3:
				{
					auto thisMaterial = std::make_shared<Glossy>();
					thisMaterial->baseColor = Color(UniformRandom(), UniformRandom(), UniformRandom());
					thisMaterial->Roughness = 0;

					ThisMesh->m_Material = thisMaterial;
				}
				break;

				case 4:
				{
					auto thisMaterial = std::make_shared<Glossy>();
					thisMaterial->baseColor = Color(UniformRandom(), UniformRandom(), UniformRandom());
					thisMaterial->Roughness = 1;

					ThisMesh->m_Material = thisMaterial;
				}
				break;

				default: break;
				}
				
				if (meshMaterial->GetTextureCount(aiTextureType_DIFFUSE))
				{
					aiString diffuseTexturePath;
					meshMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath);

					auto textureMaterial = std::make_shared<Diffuse>();
					textureMaterial->diffuseTexture = std::make_shared<Texture>();

					//Only Work On Windows
					std::string FullPath = FileDir + std::string("\\") + std::string(diffuseTexturePath.C_Str());
					if (!textureMaterial->diffuseTexture->loadFrom(FullPath.c_str()))
					{
						textureMaterial->diffuseTexture->loadFrom(diffuseTexturePath.C_Str());
					}

					ThisMesh->m_Material = textureMaterial;
				}
				
			}
		}

		m_Meshs.push_back(ThisMesh);
	}

	void ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i += 1)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(mesh, scene);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i += 1)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void Clear()
	{
		m_Meshs.clear();
	}

};