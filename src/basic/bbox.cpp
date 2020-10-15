#include"bbox.h"

bool BBox::hit(const Ray &_ray) const
{
    using std::min;
    using std::max;

    Ray ray=_ray;
    ray.normalize();

    double t_min=-inf,t_max=inf,t1,t2,inv;

    inv=1.0/ray.forward.x;
    t1=(corner_x.x-ray.origin.x)*inv;
    t2=(corner_y.x-ray.origin.x)*inv;
    t_min=max(t_min,min(t1,t2));
    t_max=min(t_max,max(t1,t2));

    inv=1.0/ray.forward.y;
    t1=(corner_x.y-ray.origin.y)*inv;
    t2=(corner_y.y-ray.origin.y)*inv;
    t_min=max(t_min,min(t1,t2));
    t_max=min(t_max,max(t1,t2));

    inv=1.0/ray.forward.z;
    t1=(corner_x.z-ray.origin.z)*inv;
    t2=(corner_y.z-ray.origin.z)*inv;
    t_min=max(t_min,min(t1,t2));
    t_max=min(t_max,max(t1,t2));

    if(t_min>t_max) return false;
    return true;
}
