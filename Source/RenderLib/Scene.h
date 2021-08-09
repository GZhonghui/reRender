#pragma once

#define OPENCV_COLOR

#include<mutex>
#include<atomic>
#include<memory>
#include<cstdio>
#include<thread>
#include<vector>
#include<chrono>
#include<cstdio>
#include<unordered_map>

#include"BVH.h"
#include"Ray.h"
#include"Model.h"
#include"Camera.h"
#include"Object.h"
#include"Skybox.h"
#include"deNoise.h"
#include"Texture.h"
#include"Semaphore.h"
#include"RenderConfig.h"
#include"MathAllInOne.h"
#include"ToolAllInOne.h"

class Scene
{
public:
	Scene()
	{
		//Generating A Scene From C++

		m_RenderThread.reset();
		//This Buffer is Controled by Python
		RenderBuffer = nullptr;

		//Create a Default Config
		m_Config = std::make_unique<RenderConfig>();

		setRendering(false);

		m_BVH = std::make_unique<BVH>(&m_Objects);
	}
	~Scene()
	{
		//Destroying A Scene From C++
	}

public:
	std::unique_ptr<RenderConfig> m_Config;

protected:
	bool m_Rendering;
	std::mutex m_RenderingLock;

	std::unique_ptr<std::thread> m_RenderThread;
	char* RenderBuffer;

	std::unique_ptr<double[]> RealRenderBuffer;
	std::mutex RealRenderBufferLock;

	unsigned int LastReadSSP;
	unsigned int RenderedSSP;

	std::vector<std::unique_ptr<std::atomic<int>>> m_Procress;

//Objects
protected:
	std::vector < std::shared_ptr<Vertex>> m_Vertices;
	std::vector<std::shared_ptr<Object>> m_Objects;
	std::unique_ptr<BVH> m_BVH;

	Camera m_Camera;

	Skybox m_Sky;

protected:
	void SubmitASSP(double* OneSSPBuffer)
	{
		std::lock_guard<std::mutex> Lock(RealRenderBufferLock);
		
		//Add 1 SSP to Total Render Buffer
		if (!RealRenderBuffer)
		{
			return;
		}

		int AllSSP = m_Config->core * m_Config->spp;
		double OneInAll = 1.0 / AllSSP;

		int BufferSize = m_Config->windowWidth * m_Config->windowHeight * 3;

		for (int i = 0; i < BufferSize; i += 1)
		{
			RealRenderBuffer[i] += OneSSPBuffer[i] * OneInAll;
		}
		RenderedSSP += 1;

		if (RenderedSSP == AllSSP)
		{
			//Final Submit
		}
	}

public:
	bool ToPythonBuffer(char* Procress)
	{
		std::lock_guard<std::mutex> Lock(RealRenderBufferLock);

		if (Procress)
		{
			memcpy(Procress, &RenderedSSP, 4);

			for (int i = 0; i < m_Config->core; i += 1)
			{
				int32_t ThisProc = *m_Procress[i];
				memcpy(Procress + (i + 1) * 4, &ThisProc, 4);
			}
		}

		if (RenderedSSP == LastReadSSP)
		{
			return false;
		}

		if (!RealRenderBuffer || !RenderBuffer)
		{
			return false;
		}

		//Float -> uint8
		int BufferSize = m_Config->windowWidth * m_Config->windowHeight * 3;
		uint8_t* Pixel = new uint8_t[BufferSize];

		int AllSSP = m_Config->core * m_Config->spp;
		double NowProcess = 1.0 * AllSSP / RenderedSSP;

		for (int i = 0; i < BufferSize; i += 1)
		{
			
			Pixel[i] = (uint8_t)(Clamp(RealRenderBuffer[i], 0, 1) * NowProcess * 255.999);
		}

		memcpy(RenderBuffer, Pixel, BufferSize);

		delete[] Pixel;

		LastReadSSP = RenderedSSP;

		return true;
	}

public:
	bool isRendering()
	{
		std::lock_guard<std::mutex> Lock(m_RenderingLock);
		return m_Rendering;
	}

	void setRendering(bool newState)
	{
		std::lock_guard<std::mutex> Lock(m_RenderingLock);
		m_Rendering = newState;
	}

protected:
	static void RenderWorkerThread(uint32_t tID, Scene* scene, Semaphore* Done);
	static void RenderThread(Scene* scene);

public:
	bool LoadSelf(const char* filePath);
	bool LoadModel(const char* filePath);
	bool LoadSkybox(const char* fileDir);
	bool buildBVH();
	bool Render(char* RenderBuffer);
	bool ClearRenderThreads();

//To deNoise
public:
	bool GenerateMap(const char* mapType, float* Buffer);
	bool deNoise(char* RenderRes, char* deNoiseRes);

public:
	Color CastRay(const Ray& ray, int CastTime = 0) const;
};