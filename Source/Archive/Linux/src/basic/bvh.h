#ifndef BVH_H
#define BVH_H

#include<vector>
#include<algorithm>

#include"message.h"
#include"object.h"
#include"bbox.h"
#include"ray.h"

class BVH;
class Intersection;

class BVHNode
{
protected:
    Object *object;
    BVHNode *left,*right;
    BBox bounding_box;
public:
    BVHNode():object(nullptr),left(nullptr),right(nullptr){}
    BVHNode(Object *object,BVHNode *left,BVHNode *right):
        object(object),left(left),right(right){}
    ~BVHNode()=default;
public:
    friend class BVH;
};

class BVH
{
protected:
    BVHNode *root;
    std::vector<Object *> *faces;
public:
    BVH():root(nullptr),faces(nullptr){}
    ~BVH()
    {
        clear();
    }
protected:
    bool hit_node(BVHNode *now,const Ray &ray,Intersection &hit_result) const;
    void build_node(BVHNode **now,int l,int r);
    void clear_node(BVHNode *now);
public:
    bool hit(const Ray &ray,Intersection &hit_result) const;
    void build(std::vector<Object *> *faces);
    void clear();
};

#endif
