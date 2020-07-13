#include"material.h"

#include<cmath>

#include"message.h"

Color Material::cal_eval(const Direction &wi,const Direction &wo,const Direction &normal)
{
    switch(material_type)
    {
        case MaterialType::DIFFUSE:
        {
            double cosalpha=normal.dot_product(wo);
            if(cosalpha>0.0)
            {
                return base_color*(1/pi);
            }else
            {
                return Color();
            }
        }
        break;
        case MaterialType::EMISSION:
        break;
        case MaterialType::WATER:
        break;
        case MaterialType::GLASS:
        break;
    }
    Message::print(MessageType::WARNING,"You got a empty eval.");
    return Color();
}
double Material::cal_pdf(const Direction &wi,const Direction &wo,const Direction &normal)
{
    switch(material_type)
    {
        case MaterialType::DIFFUSE:
        {
            if(wo.dot_product(normal)>0.0)
            {
                return 0.5/pi;
            }else
            {
                return 0;
            }
        }
        break;
        case MaterialType::EMISSION:
        break;
        case MaterialType::GLASS:
        break;
        case MaterialType::WATER:
        break;
    }
    Message::print(MessageType::WARNING,"You got a zero pdf");
    return 0;
}
Direction Material::sample(const Direction &wi,const Direction &normal)
{
    switch(material_type)
    {
        case MaterialType::DIFFUSE:
        {
            double x_1=Utils::uniform();
            double x_2=Utils::uniform();
            double z=std::abs(1-2*x_1);
            double r=std::sqrt(1-z*z);
            double phi=2*pi*x_2;
            Direction local_ray(r*std::cos(phi),r*std::sin(phi),z);

            Direction B,C;
            if(std::abs(normal[1])>std::abs(normal[2]))
            {
                double inv_len=1/std::sqrt(normal[1]*normal[1]+normal[3]*normal[3]);
                C=Direction(normal[3]*inv_len,0,-normal[1]*inv_len);
            }else
            {
                double inv_len=1/std::sqrt(normal[2]*normal[2]+normal[3]*normal[3]);
                C=Direction(0,normal[3]*inv_len,-normal[2]*inv_len);
            }
            B=C.cross_product(normal);
            return B*local_ray[1]+C*local_ray[2]+normal*local_ray[3];
        }
        break;
        case MaterialType::EMISSION:
        break;
        case MaterialType::WATER:
        break;
        case MaterialType::GLASS:
        break;
    }
    Message::print(MessageType::WARNING,"No meaning sample.");
    return Direction();
}