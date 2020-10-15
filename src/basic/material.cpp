#include"material.h"

Direction Diffuse::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    double theta=2.0*pi*Utils::uniform(),phi=pi*Utils::uniform();
    Direction dir=Direction(cos(phi),sin(phi)*cos(theta),sin(phi)*sin(theta));
    if(dir.dot_product(normal)>0)
    {
        dir=dir*-1;
    }
    sample_p=0.5/pi;
    return dir.normalized();
}

Color Diffuse::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    if(normal.dot_product(wo)>0&&normal.dot_product(wi)<0)
    {
        return base_color/pi;
    }
    return Color(0);
}

Direction DisneyDiffuse::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    double theta=2.0*pi*Utils::uniform(),phi=pi*Utils::uniform();
    Direction dir=Direction(cos(phi),sin(phi)*cos(theta),sin(phi)*sin(theta));
    if(dir.dot_product(normal)>0)
    {
        dir=dir*-1;
    }
    sample_p=0.5/pi;
    return dir.normalized();
}

Color DisneyDiffuse::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    if(normal.dot_product(wo)>0&&normal.dot_product(wi)<0)
    {
        return base_color/pi;
    }
    return Color(0);
}

Direction Metal::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    return Direction();
}

Color Metal::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    return Color();
}

Direction Glass::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    return Direction();
}

Color Glass::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    return Color();
}

Color Frosted::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    return Color();
}

Direction Frosted::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    return Direction();
}

Direction Mirror::sample(const Direction &_normal,
    const Direction &wo,double &sample_p) const
{
    Direction normal=_normal;
    normal.normalize();
    sample_p=1;
    Direction dir=wo-normal*wo.dot_product(normal)*2;
    return dir.normalized();
}

Color Mirror::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    return Color(1);
}

Direction Light::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    Message::print(MessageType::ERROR,"Dont sample wi vector of light.");
    return Direction();
}

Color Light::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    return light_color*intensity*normal.dot_product(wo);
}

Direction Textured::sample(const Direction &normal,
    const Direction &wo,double &sample_p) const
{
    double theta=2.0*pi*Utils::uniform(),phi=pi*Utils::uniform();
    Direction dir=Direction(cos(phi),sin(phi)*cos(theta),sin(phi)*sin(theta));
    if(dir.dot_product(normal)>0)
    {
        dir=dir*-1;
    }
    sample_p=0.5/pi;
    return dir.normalized();
}

Color Textured::brdf(const Direction &wi,const Direction &normal,
    const Direction &wo) const
{
    Message::print(MessageType::ERROR,"Dont cal the brdf of texture without uv.");
    return Color();
}

Color Textured::brdf_uv(const Direction &wi,const Direction &normal,const Direction &wo,
    double u,double v) const
{
    if(!texture)
    {
        return Color(1,0,0);
    }
    if(normal.dot_product(wo)>0&&normal.dot_product(wi)<0)
    {
        Coord target=A*u+B*v+C*(1-u-v);
        return texture->get_pixel(target)/pi;
    }
    return Color(0);
}
