#include"Scene.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<STB/stb_image_write.h>

#pragma warning(disable:6386)
#pragma warning(disable:6328)
#pragma warning(disable:6387)
#pragma warning(disable:6064)
#pragma warning(disable:6054)

void Scene::RenderWorkerThread(uint32_t tID, Scene* scene, Semaphore* Done)
{
	const bool enableMSAA = scene->m_Config->enableMSAA;
	const bool enableGammaCorrection = scene->m_Config->enableGammaCorrection;

	int ssp = scene->m_Config->ssp;

	const int Width = scene->m_Config->windowWidth;
	const int Height = scene->m_Config->windowHeight;

	int BufferSize = Width * Height * 3;

	std::unique_ptr<double[]> LocalRenderBuffer = std::make_unique<double[]>(BufferSize);

	for (int i = 1; i <= ssp; i += 1)
	{
		//ID Starts At 0
		*(scene->m_Procress[tID]) = 0;

		int LastProcress = 0;

		unsigned int PixelIndex = 0;

		for (int HeightIndex = 1; HeightIndex <= Height; HeightIndex += 1)
		{
			for (int WidthIndex = 1; WidthIndex <= Width; WidthIndex += 1)
			{
				auto ViewRay = scene->m_Camera.Sample(WidthIndex, HeightIndex, Width, Height);

				Color res(0, 0, 0);

				if (enableMSAA)
				{
					for (int8_t x_offset = -1; x_offset <= 0; x_offset += 1)
					{
						for (int8_t y_offset = -1; y_offset <= 0; y_offset += 1)
						{
							auto MSAAViewRay = scene->m_Camera.Sample
							(
								(WidthIndex << 1) + x_offset,
								(HeightIndex << 1) + y_offset,
								Width << 1, Height << 1
							);
							res += scene->CastRay(MSAAViewRay);
						}
					}
					res = res * 0.25;
				}
				else
				{
					//Got the Color
					res = scene->CastRay(ViewRay);
				}

				if (enableGammaCorrection)
				{
					res[0] = std::sqrt(res[0]);
					res[1] = std::sqrt(res[1]);
					res[2] = std::sqrt(res[2]);
				}

#ifdef OPENCV_COLOR
				LocalRenderBuffer[PixelIndex + 0] = res[2];
				LocalRenderBuffer[PixelIndex + 1] = res[1];
				LocalRenderBuffer[PixelIndex + 2] = res[0];
#else
				LocalRenderBuffer[PixelIndex + 0] = res[0];
				LocalRenderBuffer[PixelIndex + 1] = res[1];
				LocalRenderBuffer[PixelIndex + 2] = res[2];
#endif
				PixelIndex += 3;

				int nowProc = static_cast<int>(100.0 * PixelIndex / BufferSize);
				if (nowProc != LastProcress)
				{
					LastProcress = nowProc;
					*(scene->m_Procress[tID]) = nowProc;
				}
			}
		}
		//Submit the SSP
		scene->SubmitASSP(LocalRenderBuffer.get());
	}

	LocalRenderBuffer.release();

	Done->notify();
}

void Scene::RenderThread(Scene* scene)
{
	int core = scene->m_Config->core;

	std::vector<std::unique_ptr<std::thread>> workers;

	Semaphore waitWorkers(0);

	for (int i = 0; i < core; i += 1)
	{
		//Start A Static member Function
		workers.push_back
		(
			std::make_unique<std::thread>
			(
				Scene::RenderWorkerThread,
				static_cast<uint32_t>(i),
				scene, &waitWorkers
			)
		);
	}

	for (int i = 0; i < core; i += 1)
	{
		waitWorkers.wait();
	}

	for (int i = 0; i < core; i += 1)
	{
		if (workers[i]->joinable())
		{
			workers[i]->join();
		}
		workers[i].release();
	}
	workers.clear();

	scene->setRendering(false);
}

bool Scene::LoadSelf(const char* filePath)
{
	FILE* OBJFile = fopen(filePath, "r");
	if (!OBJFile)
	{
		Print(PrintType::ERROR, "Load %s Failed", filePath);
	}

	std::shared_ptr<Material> ThisMaterial;

	const int BufferSize = 1024;
	char ReadBuffer[BufferSize];
	while (true)
	{
		if (EOF == fscanf(OBJFile, "%s", ReadBuffer))
		{
			break;
		}

		if (!strcmp(ReadBuffer, "Material"))
		{
			//Checked Error : Broken File
			if (EOF == fscanf(OBJFile, "%s", ReadBuffer)) return false;
			//Update Material
			if (!strcmp(ReadBuffer, "Glass"))
			{
				Color baseColor;
				double Roughness;
				double IOR;
				if (EOF == fscanf(OBJFile, "%lf %lf %lf %lf %lf",
					&baseColor[0], &baseColor[1], &baseColor[2],
					&Roughness, &IOR)) return false;

				auto newMaterial = std::make_shared<Glass>();
				newMaterial->baseColor = baseColor;
				newMaterial->Roughness = Roughness;
				newMaterial->IOR = IOR;

				ThisMaterial = newMaterial;
			}
			else if (!strcmp(ReadBuffer, "Glossy"))
			{
				Color baseColor;
				double Roughness;
				if (EOF == fscanf(OBJFile, "%lf %lf %lf %lf",
					&baseColor[0], &baseColor[1], &baseColor[2],
					&Roughness)) return false;

				auto newMaterial = std::make_shared<Glossy>();
				newMaterial->baseColor = baseColor;
				newMaterial->Roughness = Roughness;

				ThisMaterial = newMaterial;
			}
			else if (!strcmp(ReadBuffer, "Diffuse"))
			{
				Color baseColor;
				if (EOF == fscanf(OBJFile, "%lf %lf %lf",
					&baseColor[0], &baseColor[1], &baseColor[2])) return false;

				auto newMaterial = std::make_shared<Diffuse>();
				newMaterial->baseColor = baseColor;

				ThisMaterial = newMaterial;
			}
		}
		else if (!strcmp(ReadBuffer, "Sphere"))
		{
			double X, Y, Z, R;
			if (EOF == fscanf(OBJFile, "%lf %lf %lf %lf", &X, &Y, &Z, &R)) return false;

			auto objSphere = std::make_shared<Sphere>(Point(X, Y, Z), R);
			objSphere->m_Material = ThisMaterial;

			m_Objects.push_back(objSphere);
		}
		else if (ReadBuffer[0] == '#')
		{
			char Read = 0;
			do
			{
				Read = fgetc(OBJFile);
			} while (Read != '\n' && Read != EOF);
		}
	}

	if (OBJFile)
	{
		fclose(OBJFile);
	}

	return true;
}

bool Scene::LoadModel(const char* filePath)
{
	Model model;
	if (!model.Load(filePath))
	{
		Print(PrintType::ERROR, "Load Model File Failed");
		return false;
	}

	for (int mesh_index = 0; mesh_index < model.m_Meshs.size(); mesh_index += 1)
	{
		auto mesh = model.m_Meshs[mesh_index];

		uint32_t AddToIndex = static_cast<uint32_t>(m_Vertices.size());

		for (int vertex_index = 0; vertex_index < mesh->Vertices.size(); vertex_index += 1)
		{
			m_Vertices.push_back(mesh->Vertices[vertex_index]);
		}

		for (int face_index = 0; face_index < mesh->Faces.size(); face_index += 1)
		{
			auto face = mesh->Faces[face_index];

			uint32_t A = face->x() + AddToIndex;
			uint32_t B = face->y() + AddToIndex;
			uint32_t C = face->z() + AddToIndex;

			auto ThisOBJ = std::make_shared<Triangle>(A, B, C, &m_Vertices);
			ThisOBJ->m_Material = mesh->m_Material;

			m_Objects.push_back(ThisOBJ);
		}
	}

	return true;
}

bool Scene::LoadSkybox(const char* fileDir)
{
	m_Sky.LoadFile(fileDir);

	return true;
}

bool Scene::buildBVH()
{
	return m_BVH->Build();
}

bool Scene::Render(char* RenderBuffer)
{
	setRendering(true);
	this->RenderBuffer = RenderBuffer;
	RenderedSSP = 0;
	LastReadSSP = 0;

	//Init Camera
	double targetPosX = m_Config->targetPosX;
	double targetPosY = m_Config->targetPosY;
	double targetPosZ = m_Config->targetPosZ;

	double cameraPosX = m_Config->cameraPosX;
	double cameraPosY = m_Config->cameraPosY;
	double cameraPosZ = m_Config->cameraPosZ;

	const Point Target(targetPosX, targetPosY, targetPosZ);
	const Point CameraLocation(cameraPosX, cameraPosY, cameraPosZ);

	double AspectRatio = 1.0 * m_Config->windowWidth / m_Config->windowHeight;

	m_Camera.ReLocate(CameraLocation);
	m_Camera.SetView(m_Config->HorizontalAngle, AspectRatio);
	m_Camera.LookAt(Target);

	int BufferSize = m_Config->windowWidth * m_Config->windowHeight * 3;
	RealRenderBuffer = std::make_unique<double[]>(BufferSize);

	for (int i = 0; i < BufferSize; i += 1)
	{
		RealRenderBuffer[i] = 0.0f;
	}

	m_Procress.clear();
	for (int i = 0; i < m_Config->core; i += 1)
	{
		m_Procress.push_back(std::make_unique<std::atomic<int>>(0));
	}

	Print(PrintType::COUNT, "Vertices=%zu & Faces=%zu", m_Vertices.size(), m_Objects.size());

	Print(PrintType::MESSAGE, "Launching Render Thread");

	m_RenderThread = std::make_unique<std::thread>(Scene::RenderThread, this);

	return true;
}

bool Scene::ClearRenderThreads()
{
	if (m_RenderThread && m_RenderThread->joinable())
	{
		m_RenderThread->join();
		m_RenderThread.release();
		m_RenderThread.reset();
	}

	ToPythonBuffer(nullptr);

	//Do not try to delete
	RenderBuffer = nullptr;

	if (RealRenderBuffer)
	{
		RealRenderBuffer.release();
	}

	return true;
}

bool Scene::GenerateMap(const char* mapType, float* Buffer)
{
	bool generateAlbedo = false;
	bool generateNormal = false;
	if (!strcmp(mapType, "albedo"))
	{
		generateAlbedo = true;
	}
	else if (!strcmp(mapType, "normal"))
	{
		generateNormal = true;
	}

	const int Width = m_Config->windowWidth;
	const int Height = m_Config->windowHeight;

	unsigned int PixelIndex = 0;
	for (int HeightIndex = 1; HeightIndex <= Height; HeightIndex += 1)
	{
		for (int WidthIndex = 1; WidthIndex <= Width; WidthIndex += 1)
		{
			auto ViewRay = m_Camera.Sample(WidthIndex, HeightIndex, Width, Height);
			Color Res;

			Intersection HitRes;
			m_BVH->Hit(ViewRay, HitRes);

			if (generateAlbedo)
			{
				if (HitRes.Hit)
				{
					if (!HitRes.HitMaterial)
					{
						Res = colorRed;
					}
					else
					{
						Res = HitRes.HitMaterial->baseColor;
					}
				}
				else
				{
					Res = m_Sky.Sample(ViewRay.forward());
				}
			}
			else if(generateNormal)
			{
				if (HitRes.Hit)
				{
					Res = HitRes.HitNormal;
				}
				else
				{
					Res = -ViewRay.forward();
				}
			}

			Buffer[PixelIndex + 0] = static_cast<float>(Res[0]);
			Buffer[PixelIndex + 1] = static_cast<float>(Res[1]);
			Buffer[PixelIndex + 2] = static_cast<float>(Res[2]);

			PixelIndex += 3;
		}
	}

	return true;
}

bool Scene::deNoise(char* RenderRes, char* deNoiseRes)
{
	deNoiser Filter;

	int BufferSize = m_Config->windowWidth * m_Config->windowHeight * 3;
	
	std::unique_ptr<float[]> inMap = std::make_unique<float[]>(BufferSize);
	std::unique_ptr<float[]> colorMap = std::make_unique<float[]>(BufferSize);
	std::unique_ptr<float[]> normalMap = std::make_unique<float[]>(BufferSize);

#ifdef OPENCV_COLOR
	for (int i = 0; i < BufferSize; i += 3)
	{
		std::swap(RenderRes[i], RenderRes[i + 2]);
	}

#endif

	std::unique_ptr<uint8_t[]> UnsignedBuffer = std::make_unique<uint8_t[]>(BufferSize);
	memcpy(UnsignedBuffer.get(), RenderRes, BufferSize);

	for (int i = 0; i < BufferSize; i += 1)
	{
		inMap[i] = static_cast<float>(UnsignedBuffer[i] / 255.999);
	}

	GenerateMap("albedo", colorMap.get());
	GenerateMap("normal", normalMap.get());

	Filter.inMap = inMap.get();
	Filter.colorMap = colorMap.get();
	Filter.normalMap = normalMap.get();
	Filter.imageWidth = m_Config->windowWidth;
	Filter.imageHeight = m_Config->windowHeight;

	Filter.deNoise(UnsignedBuffer.get());

	memcpy(deNoiseRes, UnsignedBuffer.get(), BufferSize);

#ifdef OPENCV_COLOR
	for (int i = 0; i < BufferSize; i += 3)
	{
		std::swap(RenderRes[i], RenderRes[i + 2]);
		std::swap(deNoiseRes[i], deNoiseRes[i + 2]);
	}

#endif
	
	return true;
}

Color Scene::CastRay(const Ray& ray, int CastTime) const
{
	if (CastTime > maxCastTime)
	{
		return colorBlack;
	}

	Intersection HitRes;

	if (m_BVH->Hit(ray, HitRes))
	{
		if (!HitRes.HitMaterial)
		{
			//Empty Material
			return colorRed;
		}
		else
		{
			//Get Material
			switch (HitRes.HitMaterial->GetType())
			{
			case MaterialType::EMISSION:
			{
				auto mEmission = dynamic_cast<Emission*>(HitRes.HitMaterial.get());
				if (!mEmission) return colorRed;

				return mEmission->baseColor * mEmission->Strength;
			}
			break;

			case MaterialType::DIFFUSE:
			{
				auto mDiffuse = dynamic_cast<Diffuse*>(HitRes.HitMaterial.get());
				if (!mDiffuse) return colorRed;

				Ray NextRay;
				mDiffuse->Scatter(ray, HitRes, NextRay);

				auto baseColor = mDiffuse->baseColor;
				if (mDiffuse->diffuseTexture)
				{
					baseColor = mDiffuse->diffuseTexture->Sample
					(
						HitRes.HitObject->HitPointToTextureCoords(HitRes.U, HitRes.V)
					);
				}

				//Remember to +1
				return CastRay(NextRay, CastTime + 1).cwiseProduct(baseColor);
			}
			break;

			case MaterialType::GLASS:
			{
				auto mGlass = dynamic_cast<Glass*>(HitRes.HitMaterial.get());
				if (!mGlass) return colorRed;

				Ray NextRay;
				if (mGlass->Scatter(ray, HitRes, NextRay))
				{
					return CastRay(NextRay, CastTime + 1).cwiseProduct(mGlass->baseColor);
				}
				else
				{
					return colorBlack;
				}
			}
			break;

			case MaterialType::GLOSSY:
			{
				auto mGlossy = dynamic_cast<Glossy*>(HitRes.HitMaterial.get());
				if (!mGlossy) return colorRed;

				Ray NextRay;
				if (mGlossy->Scatter(ray, HitRes, NextRay))
				{
					return CastRay(NextRay, CastTime + 1).cwiseProduct(mGlossy->baseColor);
				}
				else
				{
					return colorBlack;
				}
			}
			break;

			default:break;
			}
		}
	}

	if (!CastTime)
	{
		return m_Sky.Sample(ray.forward());
	}
	else
	{
		return m_Sky.Sample(ray.forward());
	}
}