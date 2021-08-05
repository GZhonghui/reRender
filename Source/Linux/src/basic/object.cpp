#include"object.h"

bool Triangle::intersect(const Ray &_ray,Intersection &hit_result) const
{
    Ray ray=_ray;
    ray.normalize();

    Direction AC=C-A;
    Direction AB=B-A;
    Direction AO=ray.origin-A;
    Direction normal=AC.cross_product(AB).normalized();
    bool forward_hit=normal.dot_product(ray.forward)>0?false:true;

    double t,u,v,inv=1.0/ray.forward.cross_product(AC).dot_product(AB);
    u=ray.forward.cross_product(AC).dot_product(AO)*inv;
    v=AO.cross_product(AB).dot_product(ray.forward)*inv;
    t=AO.cross_product(AB).dot_product(AC)*inv;

    double w=1-u-v;
    if(u<0||u>1) return false;
    if(v<0||v>1) return false;
    if(w<0||w>1) return false;
    if(t<eps) return false;

    hit_result=Intersection(forward_hit,t,
        ray.origin+ray.forward.normalized()*t,normal,material,1-u-v,u);
    return true;
}

void Triangle::generate_bbox(BBox &bounding_box) const
{
    using std::min;
    using std::max;
    Point corner_x,corner_y;
    corner_x.x=min(A.x,min(B.x,C.x))-box_limit;
    corner_x.y=min(A.y,min(B.y,C.y))-box_limit;
    corner_x.z=min(A.z,min(B.z,C.z))-box_limit;
    corner_y.x=max(A.x,max(B.x,C.x))+box_limit;
    corner_y.y=max(A.y,max(B.y,C.y))+box_limit;
    corner_y.z=max(A.z,max(B.z,C.z))+box_limit;
    bounding_box=BBox(corner_x,corner_y);
}

Point Triangle::center() const
{
    Point target=(B+C)/2;
    Direction AO=target-A;
    return A+AO*(2.0/3);
}

Point Triangle::sample() const
{
    double u=Utils::uniform();
    double v=Utils::uniform();
    double w=Utils::uniform();
    double sum=u+v+w;
    u=u/sum;
    v=v/sum;
    w=1-u-v;
    return Point(A*u+B*v+C*w);
}

double Triangle::area() const
{
    Direction AB=B-A;
    Direction AC=C-A;
    return AB.cross_product(AC).normal()*0.5;
}

bool Sphare::intersect(const Ray &_ray,Intersection &hit_result) const
{
    Ray ray=_ray;
    ray.normalize();

    bool forward_hit=ray.origin.distance(_center)<radius?false:true;
    
    double Delta=std::pow(ray.forward.dot_product(ray.origin-_center),2);
    Delta-=(ray.origin-_center).dot_product(ray.origin-_center);
    Delta+=std::pow(radius,2);
    if(Delta<0)
    {
        return false;
    }

    double t=-1;
    if(Delta<eps)
    {
        t=ray.forward.dot_product(ray.origin-_center)*-1;
        t=t/std::pow(ray.forward.normal(),2);
    }else
    {
        double t1=ray.forward.dot_product(ray.origin-_center)*-1;
        double t2=t1;
        t1=(t1-std::sqrt(Delta))/std::pow(ray.forward.normal(),2);
        t2=(t2+std::sqrt(Delta))/std::pow(ray.forward.normal(),2);
        if(t1<0) t=t2;
        else t=t1;
    }
    if(t<eps)
    {
        return false;
    }

    Point hit_point=ray.move(t);
    Direction normal=(hit_point-_center).normalized();
    hit_result=Intersection(forward_hit,t,hit_point,normal,material);
    return true;
}

void Sphare::generate_bbox(BBox &bounding_box) const
{
    Point corner_x=_center-Point(radius+box_limit);
    Point corner_y=_center+Point(radius+box_limit);
    bounding_box=BBox(corner_x,corner_y);
}

Point Sphare::center() const
{
    return _center;
}

Point Sphare::sample() const
{
    double x=Utils::uniform()*2-1;
    double y=Utils::uniform()*2-1;
    double z=Utils::uniform()*2-1;
    return _center+Direction(x,y,z).normalized()*radius;

//    using std::sin;
//    using std::cos;

//    double theta=2.0*pi*Utils::uniform(),phi=pi*Utils::uniform();
//    Direction dir=Direction(cos(phi),sin(phi)*cos(theta),sin(phi)*sin(theta));

//    return _center+dir.normalized()*radius;
}

double Sphare::area() const
{
    return 4*pi*radius*radius;
}
