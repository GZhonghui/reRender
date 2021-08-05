#include <cstdio>

//Remember
#include"Export.h"

std::unordered_map<int, std::unique_ptr<Scene>> Scenes;

RENDERLIB_API void LibSay()
{
    puts("This is a Function for TEST");
}

RENDERLIB_API void initAllLib()
{
    //Init Only Once
    static bool First = true;
    if (First)
    {
        //Do Init
        Print(PrintType::MESSAGE, "Initing All Lib State");

        srand((unsigned)time(nullptr));

        First = false;
    }
}

RENDERLIB_API int CreateAScene()
{
    int ID = static_cast<int>(Scenes.size()) + 1;

    //Create A Scene
    Scenes[ID] = std::make_unique<Scene>();

    return ID;
}

RENDERLIB_API int LoadAModel(int ID, const char* filePath, int type)
{
    if (!Scenes.count(ID)) return 0;

    Print(PrintType::MESSAGE, "Loading %s", filePath);

    //Self
    if (type == 1)
    {
        Scenes[ID]->LoadSelf(filePath);
    }
    //Standard OBJ file
    else if (type == 2)
    {
        Scenes[ID]->LoadModel(filePath);
    }
    else
    {
        //Unknown Type
        return 0;
    }

    return 1;
}

RENDERLIB_API int LoadSky(int ID, const char* fileDir)
{
    if (!Scenes.count(ID)) return 0;

    Scenes[ID]->LoadSkybox(fileDir);

    return 1;
}

RENDERLIB_API int buildBVH(int ID)
{
    if (!Scenes.count(ID)) return 0;

    Print(PrintType::MESSAGE, "Building BVH");

    return static_cast<int>(Scenes[ID]->buildBVH());
}

RENDERLIB_API int applyConfig(int ID,const char* config)
{
    Print(PrintType::MESSAGE, "Loading Render Config");

    if (!Scenes.count(ID)) return 0;

    int32_t ssp = 0;
    memcpy(&ssp, config, 4);
    Scenes[ID]->m_Config->ssp = ssp;

    int32_t core = 0;
    memcpy(&core, config + 4, 4);
    Scenes[ID]->m_Config->core = core;

    Print(PrintType::CONFIG, "SSP=%d & Core=%d", ssp, core);

    int32_t RenderBufferWidth = 0;
    int32_t RenderBufferHeight = 0;
    memcpy(&RenderBufferWidth, config + 8, 4);
    memcpy(&RenderBufferHeight, config + 12, 4);
    Print(PrintType::CONFIG, "Window Size=(%d,%d)", RenderBufferWidth, RenderBufferHeight);
    Scenes[ID]->m_Config->windowWidth = RenderBufferWidth;
    Scenes[ID]->m_Config->windowHeight = RenderBufferHeight;

    int32_t HorizontalAngle = 0;
    memcpy(&HorizontalAngle, config + 16, 4);
    Print(PrintType::CONFIG, "Horizontal Angle=%d", HorizontalAngle);
    Scenes[ID]->m_Config->HorizontalAngle = HorizontalAngle;

    float CameraPosX = 0;
    float CameraPosY = 0;
    float CameraPosZ = 0;
    memcpy(&CameraPosX, config + 20, 4);
    memcpy(&CameraPosY, config + 24, 4);
    memcpy(&CameraPosZ, config + 28, 4);
    Print(PrintType::CONFIG, "Put Camera At (%.2f,%.2f,%.2f)", CameraPosX, CameraPosY, CameraPosZ);
    Scenes[ID]->m_Config->cameraPosX = CameraPosX;
    Scenes[ID]->m_Config->cameraPosY = CameraPosY;
    Scenes[ID]->m_Config->cameraPosZ = CameraPosZ;

    float TargetPosX = 0;
    float TargetPosY = 0;
    float TargetPosZ = 0;
    memcpy(&TargetPosX, config + 32, 4);
    memcpy(&TargetPosY, config + 36, 4);
    memcpy(&TargetPosZ, config + 40, 4);
    Print(PrintType::CONFIG, "Let Camera Look At (%.2f,%.2f,%.2f)", TargetPosX, TargetPosY, TargetPosZ);
    Scenes[ID]->m_Config->targetPosX = TargetPosX;
    Scenes[ID]->m_Config->targetPosY = TargetPosY;
    Scenes[ID]->m_Config->targetPosZ = TargetPosZ;

    int16_t enableMSAA;
    memcpy(&enableMSAA, config + 44, 2);
    Print(PrintType::CONFIG, "EnableMSAA=%s", enableMSAA ? "True" : "False");
    Scenes[ID]->m_Config->enableMSAA = static_cast<bool>(enableMSAA);

    int16_t enableGammaCorrection;
    memcpy(&enableGammaCorrection, config + 46, 2);
    Print(PrintType::CONFIG, "EnableGammaCorrection=%s", enableGammaCorrection ? "True" : "False");
    Scenes[ID]->m_Config->enableGammaCorrection = static_cast<bool>(enableGammaCorrection);

    return 1;
}

RENDERLIB_API int Render(int ID, char* RenderBuffer)
{
    if (!Scenes.count(ID)) return 0;

    //Start a Thread
    Scenes[ID]->Render(RenderBuffer);

    return 1;
}

RENDERLIB_API int RenderDone(int ID)
{
    //Remember Why -1
    if (!Scenes.count(ID)) return -1;

    bool Result = !Scenes[ID]->isRendering();

    if (Result)
    {
        //Can be called not only once
        Scenes[ID]->ClearRenderThreads();
    }

    return static_cast<int>(Result);
}

RENDERLIB_API int UpdateRenderProgress(int ID, char* Procress)
{
    if (!Scenes.count(ID)) return 0;

    //Copy to Python Buffer
    //Will Pause Submition to Render Buffer
    return static_cast<int>(Scenes[ID]->ToPythonBuffer(Procress));
}

RENDERLIB_API int deNoise(int ID, char* RenderRes, char* deNoiseRes)
{
    if (!Scenes.count(ID)) return 0;

    return static_cast<int>(Scenes[ID]->deNoise(RenderRes, deNoiseRes));
}