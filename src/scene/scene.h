#ifndef SCENE_H
#define SCENE_H

#include"../math/vector.h"

#include"../assets/model.h"

#include"../basic/intersection.h"
#include"../basic/skybox.h"
#include"../basic/object.h"
#include"../basic/bvh.h"
#include"../basic/ray.h"

#include"../render/render_algorithm.h"

class PathTracing;
class DirectLight;
class BPathTracing;
class PhotonMapping;
class MLightTransport;

class Scene
{
protected:
    int screen_width;
    int screen_height;
    int screen_fov;
    Color background_color;
protected:
    Point camera_pos;
    Direction camera_up;
    Direction camera_dir;
    Skybox skybox;
    BVH bvh_tree;
protected:
    std::vector<Object *> lights;
    std::vector<Object *> objects;
    std::vector<Model *> models;
public:
    Scene()
    {
        Message::print(MessageType::WARNING,"You build the scene without args.");
    }
    Scene
    (
        int screen_width,
        int screen_height,
        int screen_fov,
        const Color &background_color
    ):
        screen_width(screen_width),
        screen_height(screen_height),
        screen_fov(screen_fov),
        background_color(background_color)
    {}
    ~Scene()
    {
        destroy_scene();
    }
protected:
    void load_models();
    void load_skybox();
    void find_lights();
    void build_bvh();
public:
    void load_scene();
    void build_scene();
    void clear_scene();
    void destroy_scene();
public:
    Point sample_light(double &light_p) const;
    bool intersect(const Ray &ray,Intersection &hit_result) const;
    void look_at(const Point &camera,const Point &target);
public:
    friend class Animation;
public:
    friend class Render;
    friend class RenderAlgorithm;
public:
    friend class DirectLight;
    friend class PathTracing;
    friend class BPathTracing;
    friend class PhotonMapping;
    friend class MLightTransport;
public:
    friend void path_tracing_work_render(PathTracing *render,RenderConfig *config,double *result,int *proc);
};

#endif