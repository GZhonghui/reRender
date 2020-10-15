#include"texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include"../../support/stb_image/stb_image.h"

void Texture::load_from(const char *file_name)
{
    destroy();

    unsigned char *img_data=stbi_load(file_name,&texture_width,
        &texture_height,&texture_coms,3);

    if(!img_data)
    {
        Message::print(MessageType::ERROR,"Cant load texture file.");
        return;
    }

    unsigned int pixel_buffer_size=texture_coms*texture_height*texture_width;
    pixels=new unsigned char[pixel_buffer_size];
    for(int i=0;i<pixel_buffer_size;i+=1)
    {
        pixels[i]=img_data[i];
    }
    stbi_image_free(img_data);
    
//    Message::print(MessageType::MESSAGE,"Load texture file successfully.");
}

void Texture::destroy()
{
    if(pixels)
    {
        delete[] pixels;
        pixels=nullptr;
    }
}

Color Texture::get_pixel(const Coord &coord) const
{
    return get_pixel(coord.x,coord.y);
}

Color Texture::get_pixel(double x,double y) const
{
    if(!pixels||x<0||x>1||y<0||y>1)
    {
        return Color(0);
    }
    int width_index=std::min(texture_width,(int)(x*texture_width)+1);
    int height_index=std::max(1,texture_height-(int)(y*texture_height));
    int pixel_id=3*(texture_width*(height_index-1)+width_index-1);
    return Color(pixels[pixel_id+0]/255.0,
        pixels[pixel_id+1]/255.0,pixels[pixel_id+2]/255.0);
}
