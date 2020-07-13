#ifndef RAY_H
#define RAY_H

#include"math.h"
#include"material.h"

class Triangle;
class Scene;
class Ray
{
protected:
    Point origin;
    Direction forward;
public:
    Ray()=default;
    ~Ray()=default;
    Ray(const Point &origin,const Direction &forward):
        origin(origin),forward(forward){}
    friend class Triangle;
    friend class Scene;
};

class Scene;
class Intersection
{
protected:
    bool happened;
    double distance;
    Point hit_point;
    Direction normal;
    Material material;
public:
    Intersection()=default;
    ~Intersection()=default;
    Intersection(bool happened,double distance,const Point &hit_point,
        const Direction &normal,const Material &material):
        happened(happened),distance(distance),hit_point(hit_point),
        normal(normal),material(material){}
    friend class Scene;
};

#endif