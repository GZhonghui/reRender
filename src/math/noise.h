#ifndef NOISE_H
#define NOISE_H

#include"../basic/message.h"

#include"../assets/texture.h"

#include"basic.h"

enum NoiseType
{
    RANDOM_NOISE,
    PERLIN_NOISE
};

class Noise
{
protected:
    NoiseType noise_type;
public:
    Noise():noise_type(NoiseType::RANDOM_NOISE){}
    Noise(NoiseType noise_type):noise_type(noise_type){}
    virtual ~Noise()=default;
public:
    virtual void generate()=0;
};

class Noise2D:public Noise
{
    int width;
    int height;
public:
    Noise2D():width(512),height(512){}
    Noise2D(NoiseType noise_type,int width,int height):
        Noise(noise_type),width(width),height(height){}
    virtual ~Noise2D()=default;
public:
    virtual void generate();
};

class Noise3D:public Noise
{
    int x_limit;
    int y_limit;
    int z_limit;
public:
    Noise3D():x_limit(256),y_limit(256),z_limit(256){}
    Noise3D(NoiseType noise_type,int x_limit,int y_limit,int z_limit):
        Noise(noise_type),x_limit(x_limit),y_limit(y_limit),z_limit(z_limit){}
    virtual ~Noise3D()=default;
public:
    virtual void generate();
};

#endif
