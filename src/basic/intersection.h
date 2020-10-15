#ifndef INTERSECTION_H
#define INTERSECTION_H

#include"../math/vector.h"

class Material;

class Intersection
{
protected:
    bool forward_hit;
    double distance;
    double u,v;
    Point hit_point;
    Direction normal;
    Material *material;
public:
    Intersection():distance(inf),material(nullptr){}
    Intersection(bool forward_hit,double distance,const Point &hit_point,
        const Direction &normal,Material *material,double u=-1,double v=-1):
        forward_hit(forward_hit),distance(distance),hit_point(hit_point),
        normal(normal),material(material),u(u),v(v){}
    ~Intersection()=default;
public:
    friend class BVH;
public:
    friend class MLT;
    friend class DirectLight;
    friend class PathTracing;
    friend class BPathTRacing;
    friend class PhotonMapping;
};

#endif
