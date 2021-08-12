#pragma once

#ifdef RENDERLIB2D_EXPORTS
#define RENDERLIB2D_API __declspec(dllexport)
#else
#define RENDERLIB2D_API __declspec(dllimport)
#endif

#include"ToolAIO.h"
#include"MathAIO.h"

#include"Scene.h"

extern "C" RENDERLIB2D_API void initAllLib(int Width, int Height, int Sample);

extern "C" RENDERLIB2D_API void clearAllLib();

extern "C" RENDERLIB2D_API void addObject(int Type);

extern "C" RENDERLIB2D_API void Render(const char* filePath);