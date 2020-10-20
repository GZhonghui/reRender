#ifndef PHOTON_MAPPING_H
#define PHOTON_MAPPING_H

#include<vector>

#include"../math/vector.h"

#include"../basic/ray.h"
#include"../basic/kdtree.h"

#include"render_algorithm.h"

class PhotonMapping:public RenderAlgorithm
{
protected:
    KDTree *kd_tree;
public:
    PhotonMapping():kd_tree(nullptr){}
    PhotonMapping(Scene *main_scene):RenderAlgorithm(main_scene),kd_tree(nullptr){}
    virtual ~PhotonMapping()
    {
        destroy();
    }
protected:
    void init_photons(RenderConfig *render_config);
    void destroy();
public:
    Color cast_ray(const Ray &ray);
    void cast_photon();
public:
    virtual void render(RenderConfig *render_config,double *pixels);
};

#endif