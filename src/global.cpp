#include"global.h"

#include<algorithm>
#include<cstdio>
#include<ctime>

#include"render.h"
#include"message.h"

Scene *global_state;

void py_init_lib()
{
    Message::print(MessageType::MESSAGE,"Start load the fluid lib.");

    srand((unsigned int)time(nullptr));
    global_state=nullptr;

    Message::print(MessageType::MESSAGE,"Init lib successfully.");
}
void py_set_args(int *int_args,double *double_args)
{
    int screen_width=*int_args;
    int screen_height=*(++int_args);
    int screen_fov=*(++int_args);

    int particle_size_x=*(++int_args);
    int particle_size_y=*(++int_args);
    int particle_size_z=*(++int_args);

    int box_size_x=*(++int_args);
    int box_size_y=*(++int_args);
    int box_size_z=*(++int_args);

    int ground_size=*(++int_args);

    int backgroung_color_r=*(++int_args);
    int backgroung_color_g=*(++int_args);
    int backgroung_color_b=*(++int_args);

    int core_number=*(++int_args);
    int sample_time=*(++int_args);

    int time_step=*(++int_args);

    double particle_radius=*double_args;

    double particle_start_x=*(++double_args);
    double particle_start_y=*(++double_args);
    double particle_start_z=*(++double_args);

    double box_start_x=*(++double_args);
    double box_start_y=*(++double_args);
    double box_start_z=*(++double_args);

    double camera_pos_x=*(++double_args);
    double camera_pos_y=*(++double_args);
    double camera_pos_z=*(++double_args);

    double camera_dir_x=*(++double_args);
    double camera_dir_y=*(++double_args);
    double camera_dir_z=*(++double_args);

    double camera_up_x=*(++double_args);
    double camera_up_y=*(++double_args);
    double camera_up_z=*(++double_args);

    const int buffer_size=128;
    char message_buffer[buffer_size];
    Message::print(MessageType::MESSAGE,"Receive the args.");

    sprintf(message_buffer,"Screen size:(%04d,%04d).",screen_width,screen_height);
    message_buffer[buffer_size-1]=0;
    Message::print(MessageType::MESSAGE,message_buffer);

    int particle_number=particle_size_x*particle_size_y*particle_size_z;
    sprintf(message_buffer,"Particle number:%010d.",particle_number);
    message_buffer[buffer_size-1]=0;
    Message::print(MessageType::MESSAGE,message_buffer);

    sprintf(message_buffer,"Thread number:%03d.",core_number);
    message_buffer[buffer_size-1]=0;
    Message::print(MessageType::MESSAGE,message_buffer);

    sprintf(message_buffer,"Sample time:%03d.",sample_time);
    message_buffer[buffer_size-1]=0;
    Message::print(MessageType::MESSAGE,message_buffer);

    backgroung_color_r=std::min(backgroung_color_r,255);
    backgroung_color_g=std::min(backgroung_color_g,255);
    backgroung_color_b=std::min(backgroung_color_b,255);

    if(screen_fov>160)
    {
        Message::print(MessageType::WARNING,"The value of fov is too big.");
    }else if(screen_fov<60)
    {
        Message::print(MessageType::WARNING,"The value of fov is too small.");
    }

    if(global_state)
    {
        Message::print(MessageType::ERROR,"You had set the args.");
        global_state->destroy();
        delete global_state;
    }

    global_state=new Scene
    (
        screen_width,
        screen_height,
        screen_fov,
        Box(particle_size_x,particle_size_y,particle_size_z),
        Box(box_size_x,box_size_y,box_size_z),
        ground_size,
        Color(backgroung_color_r/255.0,backgroung_color_g/255.0,backgroung_color_b/255.0),
        core_number,
        sample_time,
        time_step/1000.0,
        particle_radius,
        Point(particle_start_x,particle_start_y,particle_start_z),
        Point(box_start_x,box_start_y,box_start_z),
        Point(camera_pos_x,camera_pos_y,camera_pos_z),
        Direction(camera_dir_x,camera_dir_y,camera_dir_z).normalized(),
        Direction(camera_up_x,camera_up_y,camera_up_z).normalized()
    );
    
    Message::print(MessageType::MESSAGE,"Set the args done.");
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
        global_state->destroy();
        delete global_state;
    }
    Message::print(MessageType::MESSAGE,"Clear lib memory successfully.");
}