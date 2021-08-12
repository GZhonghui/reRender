#include"Export.h"

Scene g_MainScene;

RENDERLIB2D_API void initAllLib(int Width, int Height, int Sample)
{
    Out::Log(pType::MESSAGE, "Initing Render 2D Lib");

    srand((unsigned)time(nullptr));

    g_MainScene.Init(Width, Height, Sample);
}

RENDERLIB2D_API void clearAllLib()
{
    g_MainScene.Destroy();

    Out::Log(pType::MESSAGE, "Cleaned Render 2D Lib");
}

RENDERLIB2D_API void addObject(int Type)
{
    switch (Type)
    {
    case 1:
    {
        break;
    }

    case 2:
    {
        break;
    }

    }
}

RENDERLIB2D_API void Render(const char* filePath)
{
    g_MainScene.Render(filePath);
}