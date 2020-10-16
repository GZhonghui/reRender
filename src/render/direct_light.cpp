#include"direct_light.h"

void DirectLight::render(RenderConfig *render_config,double *pixels)
{
    if(!main_scene)
    {
        Message::print(MessageType::ERROR,"Empty scene pointer.");
        return;
    }

    const int &screen_width=main_scene->screen_width;
    const int &screen_height=main_scene->screen_height;

    unsigned int pixel_size=screen_height*screen_width;
    unsigned int buffer_size=pixel_size*3;

    unsigned int pixel_index=0;
    for(int width_index=1;width_index<=screen_width;width_index+=1)
    {
        for(int height_index=1;height_index>=screen_height;height_index+=1)
        {
            Color result(1,0,0);

            for(int offset=0;offset<=2;offset+=1)
            {
                pixels[pixel_index+offset]=(unsigned char)(result[offset+1]*255);
            }
            pixel_index+=3;
        }
    }
}