#include"Export.h"

Scene g_MainScene;

RENDERLIB2D_API void initAllLib(int Width, int Height)
{
    srand((unsigned)time(nullptr));

    g_MainScene.Init(Width, Height);
}

RENDERLIB2D_API void clearAllLib()
{
    g_MainScene.Destroy();
}

RENDERLIB2D_API void addObject()
{

}

RENDERLIB2D_API void Render()
{

}