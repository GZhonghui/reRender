#ifndef RENDER_H
#define RENDER_H

#include<vector>

#include"object.h"
#include"fluid.h"

const double russian_roulette=0.8;

class Scene
{
protected:
    int frame;
    Solver solver;
    int ground_size;
    int screen_width;
    int screen_height;
    Color background_color;
    Point camera_pos;
    Direction camera_up;
    Direction camera_dir;
    std::vector<Object *> objects;
public:
    Scene()=default;
    Scene(int ground_size,int screen_width,int screen_height,const Point &camera_pos,
        const Direction &camera_up,const Direction &camera_dir):
        ground_size(ground_size),screen_width(screen_width),screen_height(screen_height),
        camera_pos(camera_pos),camera_up(camera_up),camera_dir(camera_dir){}
    ~Scene()
    {
        clear_scene();
    }
protected:
    void sample_light(Intersection &pos,double &pdf);
    Color cast_ray(const Ray &ray);
    Intersection intersect(const Ray &ray);
public:
    void build_scene();
    void load_assets();
    void clear_scene();
    void build_bvh();
    void destroy();
    void render(double *pixels);
};

#endif