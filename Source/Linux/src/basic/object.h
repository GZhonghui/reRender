#ifndef OBJECT_H
#define OBJECT_H

#include"../assets/texture.h"

#include"ray.h"
#include"bbox.h"
#include"material.h"
#include"intersection.h"

class Material;
class Scene;

enum ObjectType
{
    TRIANGLE,
    SPHARE
};

class Object
{
protected:
    ObjectType object_type;
    Material *material;
public:
    Object():material(nullptr){}
    Object(Material *material,ObjectType object_type):
        material(material),object_type(object_type){}
    virtual ~Object()=default;
public:
    ObjectType type() const
    {
        return object_type;
    }
public:
    virtual bool intersect(const Ray &_ray,Intersection &hit_result) const=0;
    virtual void generate_bbox(BBox &bounding_box) const=0;
    virtual Point center() const=0;
    virtual Point sample() const=0;
    virtual double area() const=0;
public:
    friend class Scene;
};

class Triangle:public Object
{
protected:
    Point A,B,C;
public:
    Triangle()=default;
    Triangle(const Point &A,const Point &B,const Point &C,Material *material):
        A(A),B(B),C(C),Object(material,ObjectType::TRIANGLE){}
    virtual ~Triangle()=default;
public:
    virtual bool intersect(const Ray &_ray,Intersection &hit_result) const;
    virtual void generate_bbox(BBox &bounding_box) const;
    virtual Point center() const;
    virtual Point sample() const;
    virtual double area() const;
};

class Sphare:public Object
{
protected:
    Point _center;
    double radius;
public:
    Sphare()=default;
    Sphare(const Point &center,const double &radius,Material *material):
        _center(center),radius(radius),Object(material,ObjectType::SPHARE){}
    virtual ~Sphare()=default;
public:
    virtual bool intersect(const Ray &_ray,Intersection &hit_result) const;
    virtual void generate_bbox(BBox &bounding_box) const;
    virtual Point center() const;
    virtual Point sample() const;
    virtual double area() const;
};

#endif
