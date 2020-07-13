#ifndef MODEL_H
#define MODEL_H

#include<vector>

#include"math.h"

class Model;
class Coord
{
protected:
    double x,y;
public:
    Coord()=default;
    ~Coord()=default;
    Coord(double x,double y):x(x),y(y){}
    friend class Model;
};

class Model
{
protected:
    std::vector<Point> vertexs;
    std::vector<Direction> normals;
    std::vector<Coord> coords;
    std::vector<Triface> faces;
public:
    Model()=default;
    ~Model()=default;
    Model(const char *file_name,const char *texture_file)
    {
        load_from(file_name,texture_file);
    }
public:
    void load_from(const char *file_name,const char *texture_file);
};

#endif