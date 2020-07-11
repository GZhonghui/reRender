#include"render.h"

#include<thread>
#include<vector>

#include"math.h"
#include"material.h"

void work_render(unsigned int thread_id,Color *result,int ssp,bool *done);

void Scene::build_scene()
{
    clear_scene();
    for(int i=1;i<=ground_size;i+=1)
    {
        for(int j=1;j<=ground_size;j+=1)
        {
            Object *obj;
            Color now_color=(i+j)%2?Color(1,1,1):Color(.2,.2,.2);
            Material now_material(MaterialType::DIFFUSE,now_color);
            obj=new Triangle(Point(i-1,0,j-1),Point(i,0,j),Point(i,0,j-1),now_material);
            objects.push_back(obj);
            obj=new Triangle(Point(i,0,j),Point(i-1,0,j-1),Point(i-1,0,j),now_material);
            objects.push_back(obj);
        }
    }
    load_assets();
    solver.fill_surface(objects);
    build_bvh();
}
void Scene::load_assets()
{
    while(false);
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
    build_scene();
}
void Scene::build_bvh()
{

}
void Scene::destroy()
{
    clear_scene();
}
void Scene::sample_light(Intersection &pos,double &pdf)
{

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

    }

    double p=Utils::uniform();
    if(p<russian_roulette)
    {

    }
}
Intersection Scene::intersect(const Ray &ray)
{

}

void work_render(int thread_id,Color *result,int ssp,bool *done)
{

}