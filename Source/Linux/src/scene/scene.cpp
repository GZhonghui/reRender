#include"scene.h"

extern RenderType global_render_type;

void Scene::load_models()
{
    Model *model;

    model=new Model(Point(0),"./scene/house/glass_ball.obj");
    models.push_back(model);

    model=new Model(Point(0),"./scene/house/lights.obj");
    models.push_back(model);

    model=new Model(Point(0),"./scene/house/metal_ball.obj");
    models.push_back(model);

    model=new Model(Point(0),"./scene/house/texture_box.obj");
    models.push_back(model);

    model=new Model(Point(0),"./scene/house/wall.obj");
    models.push_back(model);
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
    //For all render type do the same thing

    load_models();

    load_skybox();
}

void Scene::build_scene()
{
    //Do different things relevated to the type of render algorithm

    clear_scene();

    for(int i=0,_size=models.size();i<_size;i+=1)
    {
        models[i]->fill(objects);
    }

    find_lights();
    
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

