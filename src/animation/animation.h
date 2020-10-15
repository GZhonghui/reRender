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
    ~Animation()=default;
public:
    void update_scene(unsigned int frame);
};

#endif
