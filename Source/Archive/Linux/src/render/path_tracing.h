#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include<thread>
#include<chrono>

#include"../math/vector.h"

#include"../basic/intersection.h"
#include"../basic/ray.h"

#include"render_algorithm.h"

class PathTracing:public RenderAlgorithm
{
protected:
    bool enable_skybox;
    double russian_roulette;
public:
    PathTracing()=default;
    PathTracing(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~PathTracing()=default;
protected:
    Color cast_ray(const Ray &ray) const;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
public:
    friend void path_tracing_work_render(PathTracing *render,RenderConfig *config,double *result,int *proc);
};

#endif