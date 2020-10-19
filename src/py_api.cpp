#include"py_api.h"

//Not a global variable
Render *global_state;

bool global_c_write_image;
bool global_random_light;
bool global_enable_skybox;

int global_core_number;
int global_sample_time;

RenderType global_render_type;

void py_init_lib()
{
    Message::print(MessageType::MESSAGE,"Start load the render lib.");

    srand((unsigned int)time(nullptr));
    global_state=nullptr;

    Message::print(MessageType::MESSAGE,"Init lib successfully.");
}

void py_set_args(int *int_args,double *double_args)
{
    int render_typei=*(int_args);

    int screen_width=*(++int_args);
    int screen_height=*(++int_args);
    int screen_fov=*(++int_args);

    int background_color_r=*(++int_args);
    int background_color_g=*(++int_args);
    int background_color_b=*(++int_args);

    int enable_skyboxi=*(++int_args);
    int encode_imagei=*(++int_args);
    int random_lighti=*(++int_args);

    int core_number=*(++int_args);
    int sample_time=*(++int_args);

    double camera_pos_x=*(double_args);
    double camera_pos_y=*(++double_args);
    double camera_pos_z=*(++double_args);

    double target_pos_x=*(++double_args);
    double target_pos_y=*(++double_args);
    double target_pos_z=*(++double_args);

    if(global_state)
    {
        Message::print(MessageType::ERROR,"You had set the args.");
        global_state->destroy_render();
        delete global_state;
    }

    char message_buffer[message_size];
    Message::print(MessageType::MESSAGE,"Receive the args.");

    switch(render_typei)
    {
        case 1:
            global_render_type=RenderType::DIRECT_LIGHT;
            sprintf(message_buffer,"Render type: direct light.");
        break;
        case 2:
            global_render_type=RenderType::PATH_TRACING;
            sprintf(message_buffer,"Render type: path tracing.");
        break;
        case 3:
            global_render_type=RenderType::PHOTON_MAPPING;
            sprintf(message_buffer,"Render type: photon mapping.");
        break;
        case 4:
            global_render_type=RenderType::B_PATH_TRACING;
            sprintf(message_buffer,"Render type: bidirectional path tracing.");
        break;
        case 5:
            global_render_type=RenderType::M_LIGHT_TRANSPORT;
            sprintf(message_buffer,"Render type: metropolis light transport.");
        break;
    }
    Message::print(MessageType::MESSAGE,message_buffer);

    global_enable_skybox=enable_skyboxi?true:false;
    global_c_write_image=encode_imagei?true:false;
    global_random_light=random_lighti?true:false;

    global_core_number=core_number;
    global_sample_time=sample_time;

    sprintf(message_buffer,"Screen size:(%d,%d).",screen_width,screen_height);
    Message::print(MessageType::MESSAGE,message_buffer);

//    sprintf(message_buffer,"Thread number:%d.",core_number);
//    Message::print(MessageType::MESSAGE,message_buffer);

//    sprintf(message_buffer,"Sample time:%d.",sample_time);
//    Message::print(MessageType::MESSAGE,message_buffer);

    sprintf(message_buffer,"Camera at position (%.2lf,%.2lf,%.2lf).",
        camera_pos_x,camera_pos_y,camera_pos_z);
    Message::print(MessageType::MESSAGE,message_buffer);

    sprintf(message_buffer,"Camera look at (%.2lf,%.2lf,%.2lf).",
        target_pos_x,target_pos_y,target_pos_z);
    Message::print(MessageType::MESSAGE,message_buffer);

    background_color_r=std::min(background_color_r,255);
    background_color_g=std::min(background_color_g,255);
    background_color_b=std::min(background_color_b,255);

//    sprintf(message_buffer,"Background color (%d,%d,%d).",
//        background_color_r,background_color_g,background_color_b);
//    Message::print(MessageType::MESSAGE,message_buffer);

    if(screen_fov>160)
    {
        Message::print(MessageType::WARNING,"The value of fov is too big.");
    }else if(screen_fov<60)
    {
        Message::print(MessageType::WARNING,"The value of fov is too small.");
    }

    Point target_pos(target_pos_x,target_pos_y,target_pos_z);
    Point camera_pos(camera_pos_x,camera_pos_y,camera_pos_z);

    global_state=new Render
    (
        screen_width,
        screen_height,
        screen_fov,
        Color
        (
            background_color_r/255.0,
            background_color_g/255.0,
            background_color_b/255.0
        ),
        camera_pos,
        target_pos
    );

    Message::print(MessageType::MESSAGE,"Set the args & build render done.");
}

void py_generate(double *pixels)
{
    if(!global_state)
    {
        Message::print(MessageType::ERROR,"You should set args first.");
        return;
    }
    global_state->render(pixels);
}

void py_clear_lib()
{
    if(global_state)
    {
        global_state->destroy_render();
        delete global_state;
    }
    Message::print(MessageType::MESSAGE,"Clear lib memory successfully.");
}
