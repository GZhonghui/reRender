#include"path_tracing.h"

//Local function
void path_tracing_work_render(PathTracing *render,RenderConfig *config,double *result,int *proc);

Color PathTracing::cast_ray(const Ray &ray) const
{
    Intersection hit_result;
    if(!main_scene->intersect(ray,hit_result))
    {
        if(enable_skybox)
            return main_scene->skybox.intersect(ray.forward);
        else
            return main_scene->background_color;
    }
    if(!hit_result.material)
    {
        return Color(1,0,0);
    }

    Point now_point=hit_result.hit_point;
    Direction now_normal=hit_result.normal;
    Material *now_material=hit_result.material;

    if(now_material->type()==MaterialType::LIGHT)
    {
        return ((Light *)(now_material))->brdf(Direction(),Direction(),Direction());
    }

    Color result(0);

    switch(now_material->type())
    {
        case MaterialType::MIRROR:
        {
            if(Utils::uniform()>russian_roulette)
            {
                double sample_p;
                Direction wi=now_material->sample(now_normal,ray.forward.opposite(),sample_p);

                Intersection hit_next;
                bool happened=main_scene->intersect(Ray(now_point,wi.opposite()),hit_next);

                if(happened)
                {
                    result=cast_ray(Ray(now_point,wi.opposite()))/russian_roulette;
                }
            }
        }
        break;

        default:
        {
            double light_p;
            Point light_target=main_scene->sample_light(light_p);
            Direction to_light=light_target-now_point;

            bool blocked=false;
            Intersection hit_light;
            if(!main_scene->intersect(Ray(now_point,to_light),hit_light))
            {
                blocked=true;
            }else if(std::abs(to_light.normal()-hit_light.distance)>eps)
            {
                blocked=true;
            }

            if(!blocked)
            {
                Color fr;
                if(now_material->type()==MaterialType::TEXTURED)
                {
                    fr=((Textured *)now_material)->brdf_uv(to_light.opposite(),now_normal,
                        ray.forward*-1,hit_result.u,hit_result.v);
                }else
                {
                    fr=now_material->brdf(to_light*-1,now_normal,ray.forward.opposite());
                }
                double light_distance=hit_light.distance*hit_light.distance;
                Color light_part=((Light *)hit_light.material)->brdf(Direction(),
                    Direction(),Direction());
                double cos_1=std::max(0.0,
                    now_normal.dot_product(to_light.normalized()));
                double cos_2=std::max(0.0,
                    hit_light.normal.dot_product(to_light.opposite().normalized()));
                light_part=light_part.hadamard_product(fr);
                light_part=light_part*cos_1/cos_2/light_distance/light_p;
                result=result+light_part;
            }

            if(Utils::uniform()>russian_roulette)
            {
                double sample_p;
                Direction wi=now_material->sample(now_normal,ray.forward*-1,sample_p);

                Intersection hit_next;
                bool happened=main_scene->intersect(Ray(now_point,wi*-1),hit_next);

                if(happened&&hit_next.material&&hit_next.material->type()!=MaterialType::LIGHT)
                {
                    Color fr;
                    if(now_material->type()==MaterialType::TEXTURED)
                    {
                        fr=((Textured *)now_material)->brdf_uv(wi,now_normal,
                            ray.forward*-1,hit_result.u,hit_result.v);
                    }else
                    {
                        fr=now_material->brdf(wi,now_normal,ray.forward*-1);
                    }
                    Color next_part=cast_ray(Ray(now_point,wi*-1));
                    double _cos=std::max(0.0,now_normal.dot_product(wi*-1));
                    next_part=next_part.hadamard_product(fr);
                    next_part=next_part*_cos/russian_roulette/sample_p;
                    result=result+next_part;
                }
            }
        }
        break;
    }

    return result;
}

void PathTracing::render(RenderConfig *render_config,double *pixels)
{
    enable_skybox=render_config->enable_skybox;
    russian_roulette=render_config->russian_roulette;

    const int &screen_width=main_scene->screen_width;
    const int &screen_height=main_scene->screen_height;
    
    const int &core_number=render_config->core_number;

    unsigned int buffer_size=screen_width*screen_height*3;

    std::vector<int *> procs;
    std::vector<double *> results;
    std::vector<std::thread *> threads;

    Message::print(MessageType::MESSAGE,"Start alloc memory.");

    for(int i=1;i<=core_number;i+=1)
    {
        int *thread_proc=new int(0);
        double *result_buffer=new double[buffer_size];
        procs.push_back(thread_proc);
        results.push_back(result_buffer);
        threads.push_back(new std::thread(path_tracing_work_render,this,
            render_config,result_buffer,thread_proc));
    }
    Message::print(MessageType::MESSAGE,"Start render.");

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        int now_proc=0,total_work=100*core_number;
        for(int i=1;i<=core_number;i+=1)
        {
            now_proc+=*(procs[i-1]);
        }
        if(now_proc==total_work) break;
        now_proc=100*now_proc/total_work;
        Message::print_bar(now_proc);
    }
    Message::print_bar(100,true);

    for(int i=1;i<=core_number;i+=1)
    {
        if(threads[i-1]->joinable())
        {
            threads[i-1]->join();
        }
    }

    Message::print(MessageType::MESSAGE,"All threads work done.");

    for(int buffer_index=1;buffer_index<=buffer_size;buffer_index+=1)
    {
        pixels[buffer_index-1]=0;
        for(int core_index=1;core_index<=core_number;core_index+=1)
        {
            pixels[buffer_index-1]+=
                results[core_index-1][buffer_index-1]/core_number;
        }
    }

    for(int i=1;i<=core_number;i+=1)
    {
        delete[] results[i-1];
        delete threads[i-1];
        delete procs[i-1];
    }

    Message::print(MessageType::MESSAGE,"Free render memory.");
}

void path_tracing_work_render(PathTracing *render,RenderConfig *config,double *result,int *proc)
{
    Scene *scene=render->main_scene;

    *proc=0;
    unsigned int buffer_size=scene->screen_width*scene->screen_height*3;
    unsigned int pixel_index=0;
    Direction camera_right=scene->camera_dir.cross_product(scene->camera_up).normalized();
    double x_offset=(scene->screen_width-1)/2.0;
    double y_offset=(scene->screen_height-1)/2.0;

    for(int height_index=1;height_index<=scene->screen_height;height_index+=1)
    {
        for(int width_index=1;width_index<=scene->screen_width;width_index+=1)
        {
            for(int pixel_offset=0;pixel_offset<=2;pixel_offset+=1)
                result[pixel_index+pixel_offset]=0;
            
            Direction lookat_x=camera_right*(width_index-x_offset);
            Direction lookat_z=scene->camera_up*(height_index-y_offset);
            Direction lookat_y=scene->camera_dir*(scene->screen_width/2.0
                /std::tan(Utils::deg2rad(scene->screen_fov/2.0)));

            Direction lookat_dir=lookat_x+lookat_y-lookat_z;
            lookat_dir.normalize();
            for(int ssp_index=1;ssp_index<=config->ssp;ssp_index+=1)
            {
                Color cast_result=render->cast_ray(Ray(scene->camera_pos,lookat_dir));
                cast_result.limit(0,1);
                for(int pixel_offset=0;pixel_offset<=2;pixel_offset+=1)
                {
                    result[pixel_index+pixel_offset]+=cast_result[pixel_offset+1]/config->ssp;
                }
            }
            pixel_index+=3;
            *proc=(int)(100*pixel_index/buffer_size);
        }
    }
    *proc=100;
}
