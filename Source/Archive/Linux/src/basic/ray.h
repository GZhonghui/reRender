#ifndef RAY_H
#define RAY_H

#include"../math/vector.h"

class Ray
{
protected:
    Point origin;
    Direction forward;
public:
    Ray()=default;
    Ray(const Point &origin,const Direction &forward):
        origin(origin),forward(forward){}
    ~Ray()=default;
public:
    void normalize()
    {
        forward.normalize();
    }
    Point move(double t) const
    {
        return origin+forward.normalized()*t;
    }
public:
    friend class Triangle;
    friend class Sphare;
    friend class BBox;
public:
    friend class MLT;
    friend class DirectLight;
    friend class PathTracing;
    friend class BPathTRacing;
    friend class PhotonMapping;
};

#endif
