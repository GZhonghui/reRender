#include"bpath_tracing.h"

void BPathTracing::render(RenderConfig *render_config,double *pixels)
{
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
    
    //B Path Tracing

}