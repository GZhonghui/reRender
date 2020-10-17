#ifndef RENDER_H
#define RENDER_H

#include<algorithm>
#include<vector>
#include<cmath>

#include"../math/vector.h"

#include"../assets/model.h"
#include"../assets/texture.h"

#include"../animation/animation.h"

#include"../scene/scene.h"

#include"../basic/message.h"

#include"direct_light.h"
#include"path_tracing.h"
#include"bpath_tracing.h"
#include"photon_mapping.h"
#include"mlight_transport.h"
#include"render_algorithm.h"

class Render
{
protected:
    Scene *main_scene;
    Animation *animation;
    RenderAlgorithm *render_algorithm;
protected:
    int frame;
public:
    Render():main_scene(nullptr),animation(nullptr),render_algorithm(nullptr)
    {
        Message::print(MessageType::ERROR,"You init the render without args.");
    }
    Render
    (
        int screen_width,
        int screen_height,
        int screen_fov,
        const Color &background_color,
        const Point &camera_position,
        const Point &camera_target
    ):frame(0)
    {
        main_scene=new Scene(screen_width,screen_height,screen_fov,background_color);
        main_scene->look_at(camera_position,camera_target);
        main_scene->load_scene();

        animation=new Animation(main_scene);
        animation->init_animation();

        render_algorithm=nullptr;

        init_render();
    }
    ~Render()
    {
        destroy_render();
    }
public:
    void render(double *pixels);
public:
    void init_render();
    void destroy_render();
};

#endif
