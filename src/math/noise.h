#ifndef NOISE_H
#define NOISE_H

#include"basic.h"

class Noise
{
public:
    Noise()=default;
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
    Noise2D(int width,int height):width(width),height(height){}
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
    Noise3D():x_limit(256),y_limit(256),z_limit(){}
    Noise3D(int x_limit,int y_limit,int z_limit):
        x_limit(x_limit),y_limit(y_limit),z_limit(z_limit){}
    virtual ~Noise3D()=default;
public:
    virtual void generate();
};

#endif
