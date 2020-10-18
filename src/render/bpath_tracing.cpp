#include"bpath_tracing.h"

void BPathTracing::render(RenderConfig *render_config,double *pixels)
{
    const int &screen_width=main_scene->screen_width;
    const int &screen_height=main_scene->screen_height;

    const int &screen_fov=main_scene->screen_fov;

    unsigned int buffer_size=screen_width*screen_height*3;
    
}