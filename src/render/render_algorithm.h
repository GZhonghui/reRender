#ifndef RENDER_ALGORITHM_H
#define RENDER_ALGORITHM_H

#include"../math/vector.h"

class Scene;

enum RenderType
{
    DIRECT_LIGHT,
    PATH_TRACING,
    B_PATH_TRACING,
    PHOTON_MAPPING,
    M_LIGHT_TRANSPORT
};

class RenderConfig
{
public:
    int ssp;
    int core_number;
    int photon_count;
    bool enable_skybox;
    double russian_roulette;
public:
    RenderConfig():
        ssp(1),
        core_number(1),
        photon_count(1000),
        enable_skybox(false),
        russian_roulette(0.5)
    {}
    RenderConfig
    (
        int ssp,
        int core_number,
        bool enable_skybox,
        double russian_roulette
    ):
        ssp(ssp),
        core_number(core_number),
        photon_count(photon_count),
        enable_skybox(enable_skybox),
        russian_roulette(russian_roulette)
    {}
    ~RenderConfig()=default;
};

class RenderAlgorithm
{
protected:
    Scene *main_scene;
public:
    RenderAlgorithm():main_scene(nullptr){}
    RenderAlgorithm(Scene *main_scene):main_scene(main_scene){}
    virtual ~RenderAlgorithm()=default;
public:
    virtual void render(RenderConfig *render_config,double *pixels)=0;
};

#include"../scene/scene.h"

#endif