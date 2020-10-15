#include"scene.h"

extern RenderType global_render_type;

void Scene::load_models()
{
    const char *models_path="./scene/house";

    Model *model;
}

void Scene::load_skybox()
{
    skybox.load_from("./texture/sky_ocean/ocean","png");
}

void Scene::find_lights()
{
    lights.clear();
    for(auto i=objects.begin();i!=objects.end();i+=1)
    {
        if((*i)->material&&(*i)->material->type()==MaterialType::LIGHT)
        {
            lights.push_back(*i);
        }
    }
}

void Scene::build_bvh()
{
    bvh_tree.build(&objects);
}

void Scene::load_scene()
{
    load_models();

    load_skybox();
}

void Scene::build_scene()
{
    clear_scene();

    for(int i=0,_size=models.size();i<_size;i+=1)
    {
        models[i]->fill(objects);
    }

    find_lights();
    
    if(global_render_type!=RenderType::DIRECT_LIGHT)
        build_bvh();
}

void Scene::clear_scene()
{
    for(int i=0,_size=objects.size();i<_size;i+=1)
    {
        if(objects[i])
        {
            delete objects[i];
        }
    }
    lights.clear();
    objects.clear();

    bvh_tree.clear();
}

void Scene::destroy_scene()
{
    clear_scene();

    for(int i=0,_size=models.size();i<_size;i+=1)
    {
        if(models[i])
        {
            delete models[i];
        }
    }
    models.clear();

    skybox.destroy();
}

bool Scene::intersect(const Ray &ray,Intersection &hit_result) const
{
    return bvh_tree.hit(ray,hit_result);
}

Point Scene::sample_light(double &light_p) const
{
    double p=Utils::uniform();
    double sum_area=0;
    for(auto i=lights.begin();i!=lights.end();i+=1)
    {
        sum_area+=(*i)->area();
    }
    light_p=1.0/sum_area;
    for(auto i=lights.begin();i!=lights.end();i+=1)
    {
        p-=(*i)->area()/sum_area;
        if(p<0)
        {
            return (*i)->sample();
        }
    }
    return Point();
}

void Scene::look_at(const Point &camera,const Point &target)
{
    camera_pos=camera;

    camera_dir=target-camera_pos;
    camera_up=Direction
    (
        std::abs(camera_dir.z)<eps?0:camera_dir.x,
        std::abs(camera_dir.z)<eps?0:camera_dir.y,
        std::abs(camera_dir.z)<eps?1:
        (camera_dir.x*camera_dir.x+camera_dir.y+camera_dir.y)/(-camera_dir.z)
    );
    if(camera_up.z<0)
    {
        camera_up=camera_up*-1;
    }

    camera_dir.normalize();
    camera_up.normalize();
}

