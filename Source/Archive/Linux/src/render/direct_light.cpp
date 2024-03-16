#include"direct_light.h"

Color DirectLight::cast_ray(const Ray &ray) const
{
    Intersection hit_result;

    if(!main_scene->intersect(ray,hit_result))
    {
        if(enable_skybox)
            return main_scene->skybox.intersect(ray.forward);
        else
            return main_scene->background_color;
    }
    
    if((!hit_result.material)||(!hit_result.forward_hit))
    {
        return missing_material;
    }

    Material *now_material=hit_result.material;

    if(now_material->type()==MaterialType::LIGHT)
    {
        return now_material->brdf(x_dir,z_dir,z_dir);
    }

    Color result(0);

    switch(now_material->type())
    {
        case MaterialType::MIRROR:
        {
            result=Color(0.95);
        }
        break;

        case MaterialType::DIFFUSE:
        {
            Direction forward_dir(1,0,0);
            Direction back_dir=forward_dir.opposite();
            result=now_material->brdf(forward_dir,back_dir,back_dir);
        }
        break;

        default:
        {
            result=Color(0);
        }
        break;
    }

    return result;
}

Color DirectLight::cast_ray_material_layer(const Ray &ray) const
{
    Intersection hit_result;

    if(!main_scene->intersect(ray,hit_result))
    {
        if(enable_skybox)
            return main_scene->skybox.intersect(ray.forward);
        else
            return main_scene->background_color;
    }
    
    if((!hit_result.material)||(!hit_result.forward_hit))
    {
        return missing_material;
    }

    Color result;

    switch(hit_result.material->type())
    {
        case MaterialType::DIFFUSE:
        {
            result=Color(0.8,0.7,0.6);
        }
        break;

        default:
        {
            result=missing_material;
        }
    }

    return result;
}

void DirectLight::render(RenderConfig *render_config,double *pixels)
{
    if(!main_scene)
    {
        Message::print(MessageType::ERROR,"Empty scene pointer.");
        return;
    }

    //Cast ray by which function
    int cast_way=1;

    enable_skybox=render_config->enable_skybox;

    const int &screen_width=main_scene->screen_width;
    const int &screen_height=main_scene->screen_height;

    const int &screen_fov=main_scene->screen_fov;

    unsigned int pixel_size=screen_height*screen_width;
    unsigned int buffer_size=pixel_size*3;

    unsigned int pixel_index=0;

    const Direction &camera_up=main_scene->camera_up;
    const Direction &camera_dir=main_scene->camera_dir;

    const Point camera_pos=main_scene->camera_pos;

    Direction camera_right=camera_dir.cross_product(camera_up).normalized();
    double x_offset=(screen_width-1)/2.0;
    double y_offset=(screen_height-1)/2.0;

    int total_work=pixel_size;
    int now_work=0;

    for(int height_index=1;height_index<=screen_height;height_index+=1)
    {
        for(int width_index=1;width_index<=screen_width;width_index+=1)
        {
            Direction lookat_x=camera_right*(width_index-x_offset);
            Direction lookat_z=camera_up*(height_index-y_offset);
            Direction lookat_y=camera_dir*(screen_width/2.0/std::tan(Utils::deg2rad(screen_fov/2.0)));

            Direction lookat_dir=lookat_x+lookat_y-lookat_z;
            lookat_dir.normalize();

            Color cast_result=missing_material;
            switch(cast_way)
            {
                case 1:
                {
                    cast_result=cast_ray(Ray(camera_pos,lookat_dir));
                }
                break;
                case 2:
                {
                    cast_result=cast_ray_material_layer(Ray(camera_pos,lookat_dir));
                }
                break;
            }
            cast_result.limit(0,1);

            for(int offset=0;offset<=2;offset+=1)
            {
                pixels[pixel_index+offset]=cast_result[offset+1];
            }
            pixel_index+=3;

            now_work+=1;
            int now_proc=100*now_work/total_work;
            Message::print_bar(now_proc);
        }
    }
    Message::print_bar(100,true);
}