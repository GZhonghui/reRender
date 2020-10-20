#include"photon_mapping.h"

void PhotonMapping::render(RenderConfig *render_config,double *pixels)
{
    if(!main_scene)
    {
        Message::print(MessageType::ERROR,"Cant render empty scene.");
        return;
    }

    const int &screen_width=main_scene->screen_width;
    const int &screen_height=main_scene->screen_height;

    const int &screen_fov=main_scene->screen_fov;

    const Direction &camera_dir=main_scene->camera_dir;
    const Direction &camera_up=main_scene->camera_up;

    const Point &camera_pos=main_scene->camera_pos;

    unsigned int pixel_size=screen_width*screen_height;
    unsigned int buffer_size=pixel_size*3;

    Direction camera_right=camera_dir.cross_product(camera_up);
    camera_right.normalize();

    init_photons(render_config);

    
}

void PhotonMapping::init_photons(RenderConfig *render_config)
{
    //Cast photons only once
    if(kd_tree) return;

    const int &photon_count=render_config->photon_count;

    std::vector<Point> hit_points;

    for(int i=1;i<=photon_count;i+=1)
    {
        double sample_light_p;
        Point light_origin=main_scene->sample_light(sample_light_p);

        
        //Record photon
    }

    kd_tree=new KDTree;
    kd_tree->build_from(hit_points);
}

void PhotonMapping::destroy()
{
    if(kd_tree)
    {
        kd_tree->destroy();
        delete kd_tree;
        kd_tree=nullptr;
    }
}