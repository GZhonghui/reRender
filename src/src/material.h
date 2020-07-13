#ifndef MATERIAL_H
#define MATERIAL_H

#include"math.h"

enum MaterialType
{
    DIFFUSE,
    EMISSION,
    GLASS,
    WATER
};

class Scene;
class Material
{
protected:
    MaterialType material_type;
    Color base_color;
public:
    Material()=default;
    ~Material()=default;
    Material(const MaterialType &material_type,const Color &base_color):
        material_type(material_type),base_color(base_color){}
public:
    Color cal_eval(const Direction &wi,const Direction &wo,const Direction &normal);
    double cal_pdf(const Direction &wi,const Direction &wo,const Direction &normal);
    Direction sample(const Direction &wi,const Direction &normal);
    friend class Scene;
};

#endif