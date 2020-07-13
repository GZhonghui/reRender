#ifndef RENDER_H
#define RENDER_H

#include<vector>

#include"object.h"
#include"fluid.h"
#include"message.h"
#include"model.h"

const double russian_roulette=0.8;

void work_render(Scene *scene,double *result);

class Scene
{
protected:
    int frame;
    Solver solver;
    int ground_size;
    int screen_width;
    int screen_height;
    int screen_fov;
    int core_number;
    int sample_time;
    Model model;
    Color background_color;
    Point camera_pos;
    Direction camera_up;
    Direction camera_dir;
    std::vector<Object *> objects;
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
        const Box &particle_size,
        const Box &box_size,
        int ground_size,
        const Color &backgroung_color,
        int core_number,
        int sample_time,
        double time_step,
        double particle_radius,
        const Point &particle_start,
        const Point &box_start,
        const Point &camera_pos,
        const Direction &camera_dir,
        const Direction &camera_up
    ):
        frame(0),
        model
        (
            "./model/model.obj",
            "./texture/base_color.png"
        ),
        screen_width(screen_width),
        screen_height(screen_height),
        screen_fov(screen_fov),
        ground_size(ground_size),
        background_color(background_color),
        core_number(core_number),
        sample_time(sample_time),
        camera_pos(camera_pos),
        camera_dir(camera_dir),
        camera_up(camera_up),
        solver
        (
            time_step,
            box_size,
            particle_size,
            box_start,
            particle_start
        )
    {}
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
    void clear_scene();
    void build_bvh();
    void destroy();
    void render(double *pixels);
    friend void work_render(Scene *scene,double *result);
};

#endif