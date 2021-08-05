#pragma once

#ifdef RENDERLIB_EXPORTS
#define RENDERLIB_API __declspec(dllexport)
#else
#define RENDERLIB_API __declspec(dllimport)
#endif

#include<ctime>
#include<mutex>
#include<memory>
#include<cstring>
#include<cstdint>
#include<unordered_map>

#include"Scene.h"
#include"RenderConfig.h"
#include"ToolAllInOne.h"
#include"MathAllInOne.h"

extern "C" RENDERLIB_API void LibSay();

extern "C" RENDERLIB_API void initAllLib();

extern "C" RENDERLIB_API int CreateAScene();

extern "C" RENDERLIB_API int LoadAModel(int ID, const char* filePath, int type);

extern "C" RENDERLIB_API int LoadSky(int ID, const char* fileDir);

extern "C" RENDERLIB_API int buildBVH(int ID);

extern "C" RENDERLIB_API int applyConfig(int ID, const char* config);

extern "C" RENDERLIB_API int Render(int ID, char* RenderBuffer);

extern "C" RENDERLIB_API int RenderDone(int ID);

extern "C" RENDERLIB_API int UpdateRenderProgress(int ID, char* Procress);

extern "C" RENDERLIB_API int deNoise(int ID, char* RenderRes, char* deNoiseRes);