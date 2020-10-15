#ifndef DIRECT_LIGHT_H
#define DIRECT_LIGHT_H

#include"render_algorithm.h"

class DirectLight:public RenderAlgorithm
{
public:
    DirectLight()=default;
    DirectLight(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~DirectLight()=default;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif