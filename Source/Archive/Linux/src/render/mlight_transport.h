#ifndef MLIGHT_TRANSPORT_H
#define MLIGHT_TRANSPORT_H

#include"render_algorithm.h"

class MLightTransport:public RenderAlgorithm
{
public:
    MLightTransport()=default;
    MLightTransport(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~MLightTransport()=default;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif