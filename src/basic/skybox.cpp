#include"skybox.h"

void Skybox::load_from(const char *file_path,const char *file_type)
{
    destroy();

    char file[message_size];

    sprintf(file,"%s_u.%s",file_path,file_type);
    textures[0]=new Texture(file);

    sprintf(file,"%s_d.%s",file_path,file_type);
    textures[1]=new Texture(file);

    sprintf(file,"%s_f.%s",file_path,file_type);
    textures[2]=new Texture(file);

    sprintf(file,"%s_b.%s",file_path,file_type);
    textures[3]=new Texture(file);

    sprintf(file,"%s_l.%s",file_path,file_type);
    textures[4]=new Texture(file);

    sprintf(file,"%s_r.%s",file_path,file_type);
    textures[5]=new Texture(file);
}

Color Skybox::intersect(const Direction &_forward) const
{
    Direction forward=_forward.normalized();

    Color result(1,0,0);

    using std::abs;
    double _x=abs(forward.x),_y=abs(forward.y),_z=abs(forward.z);
    if(_x>_y&&_x>_z)
    {
        double t=1.0/abs(forward.x);
        double y=forward.y*t,z=forward.z*t;
        if(forward.x>0)
        {
            double tx=1-(y+1)/2.0;
            double ty=(z+1)/2.0;
            if(textures[5])
            {
                result=textures[5]->get_pixel(tx,ty);
            }
        }else
        {
            double tx=(y+1)/2.0;
            double ty=(z+1)/2.0;
            if(textures[4])
            {
                result=textures[4]->get_pixel(tx,ty);
            }
        }
    }else if(_y>_x&&_y>_z)
    {
        double t=1.0/abs(forward.y);
        double x=forward.x*t,z=forward.z*t;
        if(forward.y>0)
        {
            double tx=(x+1)/2.0;
            double ty=(z+1)/2.0;
            if(textures[2])
            {
                result=textures[2]->get_pixel(tx,ty);
            }
        }else
        {
            double tx=1-(x+1)/2.0;
            double ty=(z+1)/2.0;
            if(textures[3])
            {
                result=textures[3]->get_pixel(tx,ty);
            }
        }
    }else
    {
        double t=1.0/abs(forward.z);
        double x=forward.x*t,y=forward.y*t;
        if(forward.z>0)
        {
            double tx=(x+1)/2.0;
            double ty=1-(y+1)/2.0;
            if(textures[0])
            {
                result=textures[0]->get_pixel(tx,ty);
            }
        }else
        {
            double tx=(x+1)/2.0;
            double ty=(y+1)/2.0;
            if(textures[1])
            {
                result=textures[1]->get_pixel(tx,ty);
            }
        }
    }

    return result;
}

void Skybox::destroy()
{
    for(int i=0;i<6;i+=1)
    {
        if(textures[i])
        {
            textures[i]->destroy();
            delete textures[i];
            textures[i]=nullptr;
        }
    }
}