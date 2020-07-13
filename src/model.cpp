#include"model.h"

#include"math.h"
#include"message.h"

#define STB_IMAGE_IMPLEMENTATION
#include"../support/stb_image/stb_image.h"

void Model::load_from(const char *file_name,const char *texture_file)
{
    const int buffer_size=16;
    char obj_buffer[buffer_size];

    FILE *model_file=nullptr;
    model_file=fopen(file_name,"r");
    if(!model_file)
    {
        Message::print(MessageType::ERROR,"Cannt load model file.");
        return;
    }

    fclose(model_file);
    Message::print(MessageType::MESSAGE,"Load model file successfully.");
}
