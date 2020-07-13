#include"render.h"

#include<algorithm>
#include<thread>
#include<vector>
#include<cstdio>
#include<cmath>

#include"math.h"
#include"material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../support/stb_image/stb_image_write.h"

void Scene::build_scene()
{
    clear_scene();

    for(int i=1;i<=ground_size;i+=1)
    {
        for(int j=1;j<=ground_size;j+=1)
        {
            Object *obj;
            Color now_color(0);
            int block_i=i/100+(i%100?1:0);
            int block_j=j/100+(j%100?1:0);
            if((block_i+block_j)&1)
            {
                now_color=Color(1.0,1.0,1.0);
            }else
            {
                now_color=Color(0.2,0.2,0.2);
            }
            Material now_material(MaterialType::DIFFUSE,now_color);
            obj=new Triangle(Point(i-1,0,j-1),Point(i,0,j),Point(i,0,j-1),now_material);
            objects.push_back(obj);
            obj=new Triangle(Point(i,0,j),Point(i-1,0,j-1),Point(i-1,0,j),now_material);
            objects.push_back(obj);
        }
    }
    solver.fill_surface(objects);
    
    build_bvh();

    Message::print(MessageType::MESSAGE,"Build all scene done.");
}
void Scene::clear_scene()
{
    for(int i=1;i<=objects.size();i+=1)
    {
        if(objects[i-1])
        {
            delete objects[i-1];
        }
    }
    objects.clear();
}
void Scene::render(double *pixels)
{
    frame+=1;

    build_scene();

    const int message_size=64;
    const int frame_limit=9999;
    unsigned int buffer_size=screen_width*screen_height*3;

    std::vector<double *> results;
    std::vector<std::thread *> threads;

    char alloc_memory_message[message_size];
    sprintf(alloc_memory_message,"Start alloc memory for %02d cpu core.",core_number);
    alloc_memory_message[message_size-1]=0;
    Message::print(MessageType::MESSAGE,alloc_memory_message);

    for(int i=1;i<=core_number;i+=1)
    {
        double *result_buffer=new double[buffer_size];
        results.push_back(result_buffer);
        threads.push_back(new std::thread(work_render,this,result_buffer));
    }
    
    for(int i=1;i<=core_number;i+=1)
    {
        if(threads[i-1]->joinable())
        {
            threads[i-1]->join();
        }
    }
    Message::print(MessageType::MESSAGE,"All threads work done.");

    for(int buffer_index=1;buffer_index<=buffer_size;buffer_index+=1)
    {
        pixels[buffer_index-1]=0;
        for(int core_index=1;core_index<=core_number;core_index+=1)
        {
            pixels[buffer_index-1]+=results[core_index-1][buffer_index-1]/core_number;
        }
    }

    for(int i=1;i<=core_number;i+=1)
    {
        delete[] results[i-1];
        delete threads[i-1];
    }

    char free_memory_message[message_size];
    sprintf(free_memory_message,"Free render memory of %04d cpu core done.",core_number);
    free_memory_message[message_size-1]=0;
    Message::print(MessageType::MESSAGE,free_memory_message);

    if(frame<=frame_limit)
    {
        const int file_name_size=32;
        char render_file_name[file_name_size];
        sprintf(render_file_name,"./render/Fluid_%04d.png",frame);
        render_file_name[file_name_size-1]=0;
        unsigned char *write_data=nullptr;
        write_data=new unsigned char[buffer_size];
        for(int i=1;i<=buffer_size;i+=1)
        {
            write_data[i-1]=(unsigned char)(pixels[i-1]*255);
        }
        stbi_write_png(render_file_name,screen_width,screen_height,3,write_data,0);
        delete[] write_data;
    }else
    {
        Message::print(MessageType::WARNING,"You render too much frames.");
    }

    char render_done_message[message_size];
    sprintf(render_done_message,"Render frame %04d done.",frame);
    render_done_message[message_size-1]=0;
    Message::print(MessageType::MESSAGE,render_done_message);
}
void Scene::build_bvh()
{
    Message::print(MessageType::MESSAGE,"Build bvh done.");
}
void Scene::destroy()
{
    clear_scene();
}
void Scene::sample_light(Intersection &pos,double &pdf)
{
    double emit_area=0;
    for(int i=1;i<=objects.size();i+=1)
    {
        Object *now=objects[i-1];
        if(now->material.material_type==MaterialType::EMISSION)
        {
            emit_area+=now->area;
        }
    }
    double p=Utils::uniform()*emit_area;
    double count_area=0;
    for(int i=1;i<=objects.size();i+=1)
    {
        Object *now=objects[i-1];
        if(now->material.material_type==MaterialType::EMISSION)
        {
            count_area+=now->area;
            if(count_area>=p)
            {
                now->sample(pos,pdf);
                break;
            }
        }
    }
}
Color Scene::cast_ray(const Ray &ray)
{
    Direction wo=ray.forward*-1;

    Color light_fr;
    double light_pdf;
    Intersection to_light(false,0,Point(),Direction(),Material());

    Intersection inter=intersect(ray);
    if(!inter.happened)
        return background_color;
    
    if(inter.material.material_type==MaterialType::EMISSION)
        return inter.material.base_color;
    
    sample_light(to_light,light_pdf);

    Direction ws=to_light.hit_point-inter.hit_point;

    Intersection block=intersect(Ray(inter.hit_point,ws.normalized()));

    bool blocked=false;
    if(std::abs(ws.normal()-block.distance)>eps)
        blocked=true;

    Color result(0);

    if(!blocked)
    {
        light_fr=inter.material.cal_eval(wo,ws.normalized(),inter.normal);

        double distance=ws.normal();
        double cos_a=std::max(0.0,ws.normalized().dot_product(to_light.normal));
        double cos_b=std::max(0.0,(ws*-1).normalized().dot_product(to_light.normal));
        result=result+to_light.material.base_color.hadamard_product(light_fr)
            *cos_a*cos_b*(1/distance/distance/light_pdf);
    }

    double p=Utils::uniform();
    if(p<russian_roulette)
    {
        Color obj_fr(0);
        double obj_pdf;
        Color out_ray=inter.material.sample(wo,inter.normal).normalized();
        obj_pdf=inter.material.cal_pdf(wo,out_ray,inter.normal);
        obj_fr=inter.material.cal_eval(wo,out_ray,inter.normal);
        Intersection to_obj=intersect(Ray(inter.hit_point,out_ray));

        double cos_a=std::max(0.0,out_ray.dot_product(inter.normal));
        if(to_obj.happened&&to_obj.material.material_type!=MaterialType::EMISSION)
        {
            result=result+obj_fr.hadamard_product(cast_ray(Ray(inter.hit_point,out_ray)))
                *cos_a*(1/russian_roulette/obj_pdf);
        }
    }
    return result;
}
Intersection Scene::intersect(const Ray &ray)
{
    Intersection inter_result(false,0,Point(),Direction(),Material());
    bool found=false;
    for(int i=1;i<=objects.size();i+=1)
    {
        Object *now=objects[i-1];
        Intersection now_inter=now->intersect(ray);
        if(!now_inter.happened)
            continue;
        if(!found)
        {
            inter_result=now_inter;
            found=true;
        }else if(now_inter.distance<inter_result.distance)
        {
            inter_result=now_inter;
        }
    }
    return inter_result;
}

void work_render(Scene *scene,double *result)
{
    unsigned int buffer_size=scene->screen_width*scene->screen_height*3;
    unsigned int pixel_index=0;
    Direction camera_right=scene->camera_dir.cross_product(scene->camera_up).normalized();
    double x_offset=(scene->screen_width-1)/2.0;
    double y_offset=(scene->screen_height-1)/2.0;

    for(int height_index=1;height_index<=scene->screen_height;height_index+=1)
    {
        for(int width_index=1;width_index<=scene->screen_width;width_index+=1)
        {
            for(int pixel_offset=0;pixel_offset<=2;pixel_offset+=1)
                result[pixel_index+pixel_offset]=0;
            for(int ssp_index=1;ssp_index<=scene->sample_time;ssp_index+=1)
            {
                Direction lookat_x=camera_right*(width_index-x_offset);
                Direction lookat_y=scene->camera_up*(height_index-y_offset);
                Direction lookat_z=scene->camera_dir*(scene->screen_width/2.0
                    /std::tan(Utils::deg2rad(scene->screen_fov/2.0)));
                Direction lookat_dir=lookat_x+lookat_y+lookat_z;
                Color cast_result=scene->cast_ray(Ray(scene->camera_pos,lookat_dir.normalized()));
                for(int pixel_offset=0;pixel_offset<=2;pixel_offset+=1)
                    result[pixel_index+pixel_offset]+=cast_result[pixel_offset+1]/scene->sample_time;
            }
            pixel_index+=3;
        }
        Message::print(MessageType::ERROR,"A thread complate a line.");
    }
}