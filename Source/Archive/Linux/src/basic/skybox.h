#ifndef SKYBOX_H
#define SKYBOX_H

#include"../assets/texture.h"

#include"ray.h"

class Skybox
{
protected:
    Texture *textures[6];
public:
    Skybox()
    {
        for(int i=0;i<6;i+=1) textures[i]=nullptr;
    }
    Skybox(const char *file_path,const char *file_type)
    {
        load_from(file_path,file_type);
    }
    ~Skybox()
    {
        destroy();
    }
public:
    void load_from(const char *file_path,const char *file_type);
    Color intersect(const Direction &_forward) const;
    void destroy();
};

#endif