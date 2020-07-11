#include"object.h"

#include<cmath>

Intersection Triangle::intersect(const Ray &ray)
{
    Intersection miss(false,0,Point(),Direction(),Material());
    if(ray.forward.dot_product(normal)>0.)
        return miss;
    
    double u,v,t;
    Direction pvec=ray.forward.cross_product(AC);

    double det=AB.dot_product(pvec);
    if(std::abs(det)<eps)
        return miss;
    
    double det_inv=1./det;
    Direction tvec=ray.origin-A;
    u=tvec.dot_product(pvec)*det_inv;
    if(u<0||u>1)
        return miss;
    
    Direction qvec=tvec.cross_product(AB);
    v=ray.forward.dot_product(qvec)*det_inv;
    if(v<0||u+v>1)
        return miss;

    t=AC.dot_product(qvec)*det_inv;
    if(t<0)
        return miss;

    return Intersection(true,t,ray.origin+ray.forward*t,normal,material);
}
void Triangle::sample(Intersection &pos,double &pdf)
{
    double x=std::sqrt(Utils::uniform());
    double y=Utils::uniform();
    Point hit_point=A*(1.-x)+B*(x*(1.-y))+C*(x*y);
    pos=Intersection(true,0.,hit_point,normal,material);
    pdf=1./area;
}

Intersection Sphare::intersect(const Ray &ray)
{
    return Intersection();
}
void Sphare::sample(Intersection &pos,double &pdf)
{
    pos=Intersection();
    pdf=0.;
    return;
}