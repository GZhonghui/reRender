#ifndef BBOX_H
#define BBOX_H

#include<algorithm>

#include"../math/vector.h"

#include"ray.h"

const double box_limit=1e-6;

class BBox
{
protected:
    Point corner_x;
    Point corner_y;
public:
    BBox()=default;
    BBox(const Point &corner_x,const Point &corner_y):
        corner_x(corner_x),corner_y(corner_y){}
    BBox(const BBox &box_a,const BBox &box_b)
    {
        using std::min;
        using std::max;
        corner_x.x=min(box_a.corner_x.x,box_b.corner_x.x);
        corner_x.y=min(box_a.corner_x.y,box_b.corner_x.y);
        corner_x.z=min(box_a.corner_x.z,box_b.corner_x.z);
        corner_y.x=max(box_a.corner_y.x,box_b.corner_y.x);
        corner_y.y=max(box_a.corner_y.y,box_b.corner_y.y);
        corner_y.z=max(box_a.corner_y.z,box_b.corner_y.z);
    }
    ~BBox()=default;
public:
    bool hit(const Ray &_ray) const;
};

#endif
