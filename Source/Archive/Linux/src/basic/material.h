#ifndef MATERIAL_H
#define MATERIAL_H

#include"../math/vector.h"

#include"../assets/texture.h"

const Color missing_material(0.92,0.08,0.12);

enum MaterialType
{
    DIFFUSE,
    DISNEY_DIFFUSE,
    METAL,
    GLASS,
    FROSTED,
    MIRROR,
    LIGHT,
    TEXTURED
};

class Material
{
protected:
    MaterialType material_type;
public:
    Material(MaterialType material_type):material_type(material_type){}
    virtual ~Material()=default;
public:
    MaterialType type() const
    {
        return material_type;
    }
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const=0;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const=0;
};

class Diffuse:public Material
{
protected:
    Color base_color;
public:
    Diffuse():Material(MaterialType::DIFFUSE){}
    Diffuse(const Color &base_color):base_color(base_color),
        Material(MaterialType::DIFFUSE){}
    virtual ~Diffuse()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class DisneyDiffuse:public Material
{
protected:
    Color base_color;
public:
    DisneyDiffuse():Material(MaterialType::DISNEY_DIFFUSE){}
    DisneyDiffuse(const Color &base_color):base_color(base_color),
        Material(MaterialType::DISNEY_DIFFUSE){}
    virtual ~DisneyDiffuse()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class Metal:public Material
{
protected:
    Color base_color;
    double roughness;
public:
    Metal():Material(MaterialType::METAL){}
    Metal(const Color &base_color,double roughness):
        base_color(base_color),roughness(roughness),
        Material(MaterialType::METAL){}
    virtual ~Metal()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class Glass:public Material
{
protected:
    Color base_color;
    double ref_index;
public:
    Glass():Material(MaterialType::GLASS){}
    Glass(const Color &base_color,double ref_index):
        base_color(base_color),ref_index(ref_index),
        Material(MaterialType::GLASS){}
    virtual ~Glass()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class Frosted:public Material
{
public:
    Frosted():Material(MaterialType::FROSTED){}
    virtual ~Frosted()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class Mirror:public Material
{
protected:
public:
    Mirror():Material(MaterialType::MIRROR){}
    virtual ~Mirror()=default;
public:
    virtual Direction sample(const Direction &_normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

class Light:public Material
{
protected:
    double intensity;
    Color light_color;
public:
    Light():light_color(1),intensity(1),
        Material(MaterialType::LIGHT){}
    Light(const Color &light_color,double intensity):
        light_color(light_color),intensity(intensity),
        Material(MaterialType::LIGHT){}
    virtual ~Light()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
};

//Only for triangle
class Textured:public Material
{
protected:
    Texture *texture;
    Coord A,B,C;
public:
    Textured():texture(nullptr),Material(MaterialType::TEXTURED){}
    Textured(const Coord &A,const Coord &B,const Coord &C,Texture *texture):
        A(A),B(B),C(C),texture(texture),Material(MaterialType::TEXTURED){}
    virtual ~Textured()=default;
public:
    virtual Direction sample(const Direction &normal,
        const Direction &wo,double &sample_p) const;
    virtual Color brdf(const Direction &wi,const Direction &normal,
        const Direction &wo) const;
public:
    Color brdf_uv(const Direction &wi,const Direction &normal,const Direction &wo,
        double u,double v) const;
public:
    Texture *used_texture() const
    {
        return texture;
    }
};

#endif
