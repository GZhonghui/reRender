#include"render.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../../support/stb_image/stb_image_write.h"

extern bool global_c_write_image;
extern bool gloabl_random_light;
extern bool global_enable_skybox;

extern int global_core_number;
extern int global_sample_time;

extern RenderType global_render_type;

void Render::init_render()
{
    main_scene->load_scene();

    switch(global_render_type)
    {
        case RenderType::DIRECT_LIGHT:
            render_algorithm=new DirectLight(main_scene);
        break;
        case RenderType::PATH_TRACING:
            render_algorithm=new PathTracing(main_scene);
        break;
        case RenderType::B_PATH_TRACING:
            render_algorithm=new BPathTracing(main_scene);
        break;
        case RenderType::PHOTON_MAPPING:
            render_algorithm=new PhotonMapping(main_scene);
        break;
        case RenderType::M_LIGHT_TRANSPORT:
            render_algorithm=new MLightTransport(main_scene);
        break;
    }
}

void Render::destroy_render()
{
    if(main_scene)
    {
        main_scene->destroy_scene();
        delete main_scene;
    }

    if(animation) delete animation;

    if(render_algorithm) delete render_algorithm;
}

void Render::render(double *pixels)
{
    RenderConfig config;
    config.russian_roulette=0.8;
    config.ssp=global_sample_time;
    config.core_number=global_core_number;
    config.enable_skybox=global_enable_skybox;

    frame+=1;
    animation->update_scene(frame);
    render_algorithm->render(&config,pixels);

    const int frame_limit=9999;
    if(global_c_write_image)
    {
        if(frame<=frame_limit)
        {
            char render_file_name[message_size];
            sprintf(render_file_name,"./render/C_Render_%04d.png",frame);

            const int &screen_width=main_scene->screen_width;
            const int &screen_height=main_scene->screen_height;
            unsigned char *write_data=nullptr;
            unsigned int buffer_size=screen_width*screen_height*3;
            write_data=new unsigned char[buffer_size];
            for(int i=1;i<=buffer_size;i+=1)
            {
                pixels[i-1]=std::max(0.0,std::min(1.0,pixels[i-1]));
                write_data[i-1]=(unsigned char)(pixels[i-1]*255);
            }
            stbi_write_png(render_file_name,screen_width,screen_height,3,write_data,0);
            delete[] write_data;
        }else
        {
            Message::print(MessageType::WARNING,"You render too much frames.");
        }
    }

    char render_done_message[message_size];
    sprintf(render_done_message,"Render frame %d done.",frame);
    Message::print(MessageType::MESSAGE,render_done_message);
}
