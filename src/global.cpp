#include"global.h"

#include<algorithm>
#include<ctime>

#include"render.h"
#include"message.h"

Scene *global_state;

void py_init_lib()
{
    Message::print(MessageType::MESSAGE,"Start load the fluid lib.");

    srand((unsigned int)time(nullptr));
    Message::print(MessageType::MESSAGE,"Init lib successfully.");
}
void py_set_args(int screen_width,int screen_height,int screen_fov,
    int particle_size_x,int particle_size_y,int particle_size_z,
    int particle_start_x,int particle_start_y,int particle_start_z,
    int box_size_x,int box_size_y,int box_size_z,
    int box_start_x,int box_start_y,int box_start_z,
    int ground_size,int backgroung_color_r,int backgroung_color_g,int backgroung_color_b,
    int camera_pos_x,int camera_pos_y,int camera_pos_z,
    int camera_dir_x,int camera_dir_y,int camera_dir_z,
    int camera_up_x,int camera_up_y,int camera_up_z,
    int core_number,int sample_time,
    int time_step)
{
    const int buffer_size=128;
    char message_buffer[buffer_size];
    Message::print(MessageType::MESSAGE,"Receive the args.");

    

    global_state=new Scene();
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