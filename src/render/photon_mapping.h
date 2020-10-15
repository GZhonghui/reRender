#ifndef PHOTON_MAPPING_H
#define PHOTON_MAPPING_H

#include"render_algorithm.h"

class PhotonMapping:public RenderAlgorithm
{
public:
    PhotonMapping()=default;
    PhotonMapping(Scene *main_scene):RenderAlgorithm(main_scene){}
    virtual ~PhotonMapping()=default;
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif