#ifndef OBJECT_H
#define OBJECT_H

#include"material.h"
#include"ray.h"
#include"math.h"

class Scene;
class Object
{
protected:
    Material material;
    double area;
public:
    Object()=default;
    ~Object()=default;
    Object(const Material &material):material(material){}
    virtual Intersection intersect(const Ray &ray)=0;
    virtual void sample(Intersection &pos,double &pdf)=0;
    friend class Scene;
};

class Triangle:public Object
{
protected:
    Direction normal;
    Point A,B,C;
    Direction AB,AC;
public:
    Triangle()=default;
    ~Triangle()=default;
    Triangle(const Point &A,const Point &B,const Point &C,const Material &material):
        A(A),B(B),C(C),Object(material)
    {
        AB=B-A;
        AC=C-A;
        normal=AC.cross_product(AB).normalized();
        area=AB.cross_product(AC).normal()*0.5;
    }
public:
    virtual Intersection intersect(const Ray &ray);
    virtual void sample(Intersection &pos,double &pdf);
};

class Sphare:public Object
{
protected:
    Point center;
    double radius;
public:
    Sphare()=default;
    ~Sphare()=default;
    Sphare(const Point &center,const double &radius):
        center(center),radius(radius)
    {
        area=4*pi*radius*radius;
    }
public:
    virtual Intersection intersect(const Ray &ray);
    virtual void sample(Intersection &pos,double &pdf);
};

#endif