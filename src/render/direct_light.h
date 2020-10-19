#ifndef DIRECT_LIGHT_H
#define DIRECT_LIGHT_H

#include"../math/vector.h"

#include"../basic/ray.h"
#include"../basic/material.h"

#include"render_algorithm.h"

class DirectLight:public RenderAlgorithm
{
protected:
    bool enable_skybox;
public:
    DirectLight()=default;
    DirectLight(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~DirectLight()=default;
protected:
    Color cast_ray(const Ray &ray) const;
    Color cast_ray_material_layer(const Ray &ray) const;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif