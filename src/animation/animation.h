#ifndef ANIMATION_H
#define ANIMATION_H

#include"../basic/message.h"

#include"../scene/scene.h"

class Animation
{
protected:
    Scene *main_scene;
public:
    Animation():main_scene(nullptr){}
    Animation(Scene *main_scene):main_scene(main_scene){}
    ~Animation()
    {
        destroy_animation();
    }
public:
    void update_scene(unsigned int frame);
public:
    void init_animation();
    void destroy_animation();
};

#endif
