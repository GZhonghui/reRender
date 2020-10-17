#ifndef MODEL_H
#define MODEL_H

#include<cmath>
#include<vector>
#include<cstring>
#include<unordered_map>

#include"../math/vector.h"

#include"../basic/material.h"
#include"../basic/message.h"
#include"../basic/object.h"

#include"texture.h"

class Scene;

class TriangleRow
{
public:
    Triface points;
    Material *material;
public:
    TriangleRow()=default;
    TriangleRow(const Triface &points,Material *material):
        points(points),material(material){}
    ~TriangleRow()=default;
};

class SphareRow
{
public:
    int center;
    double radius;
    Material *material;
public:
    SphareRow()=default;
    SphareRow(int center,double radius,Material *material):
        center(center),radius(radius),material(material){}
    ~SphareRow()=default;
};

class Model
{
protected:
    Point global_location;
    Direction forward_dir;
    Direction up_dir;
    std::unordered_map<int,Point> vertexs;
    std::unordered_map<int,Coord> coords;
    std::vector<Texture *> textures;
    std::vector<Material *> materials;
    std::vector<TriangleRow> triangle_faces;
    std::vector<SphareRow> sphare_faces;
public:
    Model():global_location(0),forward_dir(0,1,0),up_dir(0,0,1){}
    Model(const Point &global_location,const char *file_name):
        global_location(global_location),forward_dir(0,1,0),up_dir(0,0,1)
    {
        load_from(file_name);
    }
    ~Model()
    {
        destroy();
    }
protected:
    Direction right() const;
public:
    void move_local(const Direction &move_dir);
    void rotate_local(const Direction &angles);
    void move_global(const Direction &move_dir);
    void rotate_global(const Direction &angles);
public:
    void load_from(const char *file_name);
    void fill(std::vector<Object *> &objects);
    void destroy();
public:
    friend class Scene;
};

#endif
