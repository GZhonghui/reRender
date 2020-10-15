#ifndef B_PATH_TRACING_H
#define B_PATH_TRACING_H

#include"render_algorithm.h"

class BPathTracing:public RenderAlgorithm
{
public:
    BPathTracing()=default;
    BPathTracing(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~BPathTracing()=default;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif