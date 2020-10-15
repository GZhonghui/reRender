#include"model.h"

//Local function
bool generate_material(FILE *model_file,Material **material,std::vector<Texture *> &textures);

void Model::move_local(const Direction &move_dir)
{
    for(auto i=vertexs.begin();i!=vertexs.end();++i)
    {
        i->second=i->second+move_dir;
    }
}

void Model::rotate_local(const Direction &angles)
{
    //Apply on all points
}

void Model::move_global(const Direction &move_dir)
{
    global_location=global_location+move_dir;
}

void Model::rotate_global(const Direction &angles)
{
    //Rotate the dir
}

Direction Model::right() const
{
    Direction _right=forward_dir.cross_product(up_dir);
    return _right.normalized();
}

void Model::load_from(const char *file_name)
{
    destroy();

    char obj_buffer[message_size];

    FILE *model_file=nullptr;
    model_file=fopen(file_name,"r");
    if(!model_file)
    {
        Message::print(MessageType::ERROR,"Cannt load model file.");
        return;
    }

    Material *now_material=nullptr;

    while(EOF!=fscanf(model_file,"%s",obj_buffer))
    {
        switch(*obj_buffer)
        {
            case 'm':
            {
                if(generate_material(model_file,&now_material,textures))
                {
                    materials.push_back(now_material);
                }
            }
            break;

            case 'f':
            {
                int x,y,z;
                fscanf(model_file,"%d %d %d",&x,&y,&z);
                if(now_material&&now_material->type()==MaterialType::TEXTURED)
                {
                    Material *new_material=new Textured(coords[x],coords[y],
                        coords[z],((Textured *)now_material)->used_texture());
                    materials.push_back(new_material);
                    triangle_faces.push_back(TriangleRow(Triface(x,y,z),new_material));
                }else
                {
                    triangle_faces.push_back(TriangleRow(Triface(x,y,z),now_material));
                }
            }
            break;

            case 'b':
            {
                int center_id;
                double radius;
                fscanf(model_file,"%d %lf",&center_id,&radius);
                sphare_faces.push_back(SphareRow(center_id,radius,now_material));
            }
            break;

            case 't':
            {
                int id;
                double x,y;
                fscanf(model_file,"%d %lf %lf",&id,&x,&y);
                coords[id]=Coord(x,y);
            }
            break;

            case 'v':
            {
                int id;
                double x,y,z;
                fscanf(model_file,"%d %lf %lf %lf",&id,&x,&y,&z);
                vertexs[id]=Point(x,y,z);
            }
            break;
        }
    }

    fclose(model_file);

    Message::print(MessageType::MESSAGE,"Load model file successfully.");

    char model_info_message[message_size];

    sprintf(model_info_message,"Model vertexs count:%d.",(int)vertexs.size());
    Message::print(MessageType::MESSAGE,model_info_message);

    sprintf(model_info_message,"Model faces count:%d.",
        (int)(triangle_faces.size()+sphare_faces.size()));
    Message::print(MessageType::MESSAGE,model_info_message);
}

void Model::fill(std::vector<Object *> &objects)
{
    for(int i=0,_size=triangle_faces.size();i<_size;i+=1)
    {
        const TriangleRow &now=triangle_faces[i];
        objects.push_back(new Triangle(
            vertexs[now.points.x]+global_location,
            vertexs[now.points.y]+global_location,
            vertexs[now.points.z]+global_location,
            now.material));
    }
    for(int i=0,_size=sphare_faces.size();i<_size;i+=1)
    {
        const SphareRow &now=sphare_faces[i];
        objects.push_back(new Sphare(vertexs[now.center]+global_location,
            now.radius,now.material));
    }
}

void Model::destroy()
{
    for(int i=0,_size=textures.size();i<_size;i+=1)
    {
        if(textures[i])
        {
            delete textures[i];
        }
    }
    textures.clear();

    for(int i=0,_size=materials.size();i<_size;i+=1)
    {
        if(materials[i])
        {
            delete materials[i];
        }
    }
    materials.clear();

    vertexs.clear();
    coords.clear();

    triangle_faces.clear();
    sphare_faces.clear();
}

bool generate_material(FILE *model_file,Material **material,std::vector<Texture *> &textures)
{
    char obj_buffer[message_size];
    fscanf(model_file,"%s",obj_buffer);
    if(!strcmp(obj_buffer,"DIFFUSE"))
    {
        int r,b,g;
        fscanf(model_file,"%d %d %d",&r,&g,&b);
        *material=new Diffuse(Color(r/255.0,g/255.0,b/255.0));
    }else if(!strcmp(obj_buffer,"DISNEY_DIFFUSE"))
    {
        int r,b,g;
        fscanf(model_file,"%d %d %d",&r,&g,&b);
        *material=new DisneyDiffuse(Color(r/255.0,g/255.0,b/255.0));
    }else if(!strcmp(obj_buffer,"METAL"))
    {
        *material=new Metal();
    }else if(!strcmp(obj_buffer,"GLASS"))
    {
        *material=new Glass();
    }else if(!strcmp(obj_buffer,"FROSTED"))
    {
        *material=new Frosted();
    }else if(!strcmp(obj_buffer,"MIRROR"))
    {
        *material=new Mirror();
    }else if(!strcmp(obj_buffer,"LIGHT"))
    {
        int r,b,g;
        double intensity;
        fscanf(model_file,"%d %d %d %lf",&r,&g,&b,&intensity);
        *material=new Light(Color(r/255.0,g/255.0,b/255.0),intensity);
    }else if(!strcmp(obj_buffer,"TEXTURED"))
    {
        char texture_file[message_size];
        fscanf(model_file,"%s",texture_file);
        Texture *texture=new Texture(texture_file);
        textures.push_back(texture);
        *material=new Textured(Coord(),Coord(),Coord(),texture);
    }else
    {
        *material=nullptr;
        Message::print(MessageType::ERROR,obj_buffer);
        Message::print(MessageType::ERROR,"There is a error in model file.");
        return false;
    }
    return true;
}
