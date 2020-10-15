#ifndef TEXTURE_H
#define TEXTURE_H

#include"../math/vector.h"

#include"../basic/message.h"

class Texture
{
protected:
    unsigned char *pixels;
    int texture_height;
    int texture_width;
    int texture_coms;
public:
    Texture():pixels(nullptr){}
    Texture(const char *file_name)
    {
        pixels=nullptr;
        load_from(file_name);
    }
    ~Texture()
    {
        destroy();
    }
public:
    Color get_pixel(double x,double y) const;
    Color get_pixel(const Coord &coord) const;
public:
    void load_from(const char *file_name);
    void destroy();
};

#endif
