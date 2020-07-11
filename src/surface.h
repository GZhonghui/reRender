#ifndef SURFACE_H
#define SURFACE_H

#include<vector>

#include"object.h"
#include"math.h"

class Surface
{
protected:
public:
    Surface()=default;
    ~Surface()=default;
public:
    void fill_surface(const std::vector<Point> &particles,std::vector<Object *> &objects);
};

#endif