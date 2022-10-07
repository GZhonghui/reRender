#include "ServerProcessor.h"

#include "RenderMessage.pb.h"

// Copied from main.cc

//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "aarect.h"
#include "box.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "GSkybox.h"

#include <iostream>
#include <fstream>

extern Skybox Sky;

color ray_color(
    const ray& r,
    const color& background,
    const hittable& world,
    shared_ptr<hittable> lights,
    int depth
) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;

        // auto R = Sky.Sample(Direction(r.dir.x(), r.dir.y(), r.dir.z()));
        // return color(R[0], R[1], R[2]);
    }

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
             * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
         + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                            * ray_color(scattered, background, world, lights, depth-1)
                            / pdf_val;
}


hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    auto yellow = make_shared<dielectric>(1.4f);
    
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));

    objects.add(make_shared<triangle>(vec3(278 -100, 20, 200), vec3(278 + 0, 100, 300), vec3(278 + 100, 20, 200), yellow));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190,90,190), 90 , glass));
    
    return objects;
}

void ServerProcessor::run()
{
    // Image

    // Edited
    // const auto aspect_ratio = 1.0 / 1.0;
    // const int image_width = 600;
    // const int image_height = static_cast<int>(image_width / aspect_ratio);

    const auto aspect_ratio = 1.0 * RenderWidth / RenderHeight;
    const int image_width = RenderWidth;
    const int image_height = RenderHeight;

    int samples_per_pixel = Spp;
    int max_depth = 50;

    // Build world & light
    // hittable_list world;

    hittable_list world;

    auto lights = make_shared<hittable_list>();
    bool has_light = false;
    bool has_fog = false;

    auto default_mat = make_shared<lambertian>(color(.10, .05, .05));

    std::unordered_map<std::string, std::shared_ptr<material>> mat_table;
    std::unordered_map<std::string, bool> mat_table_is_light;
    std::unordered_map<std::string, bool> mat_table_is_fog;

    {
        for(int i=0;i<SceneData->materials_size();i+=1)
        {
            Out::Log(pType::MESSAGE, "Material Count = %d", SceneData->materials_size());
            auto Mat = SceneData->materials(i);

            std::string MatName = Mat.name();
            if(Mat.type() == 1) // Diffuse
            {
                float red = Mat.parameters(0);
                float green = Mat.parameters(1);
                float blue = Mat.parameters(2);

                mat_table[MatName] = std::make_shared<lambertian>(color(red, green, blue));
                mat_table_is_light[MatName] = false;
            }else if(Mat.type() == 2) // Metal
            {
                float red = Mat.parameters(0);
                float green = Mat.parameters(1);
                float blue = Mat.parameters(2);
                float fuzzy = Mat.parameters(3);

                mat_table[MatName] = std::make_shared<metal>(color(red, green, blue), fuzzy);
                mat_table_is_light[MatName] = false;
            }else if(Mat.type() == 3) // Glass
            {
                float IOR = Mat.parameters(0);

                mat_table[MatName] = std::make_shared<dielectric>(IOR);
                mat_table_is_light[MatName] = false;
            }else if(Mat.type() == 4) // Emit
            {
                float red = Mat.parameters(0);
                float green = Mat.parameters(1);
                float blue = Mat.parameters(2);
                float intensity = Mat.parameters(3);

                mat_table[MatName] = std::make_shared<diffuse_light>(color(red, green, blue) * intensity);
                mat_table_is_light[MatName] = true;
            }else if(Mat.type() == 5)
            {
                mat_table[MatName] = default_mat; // Fake Fog
                mat_table_is_light[MatName] = false;

                mat_table_is_fog[MatName] = true;
            }
        }

        for(int i=0;i<SceneData->meshs_size();i+=1)
        {
            auto Mesh = SceneData->meshs(i);
            auto MatName = Mesh.matname();

            std::shared_ptr<material> ThisMat = default_mat;
            bool isLight = false;
            bool isFog = false;

            if(mat_table.count(MatName))
            {
                ThisMat = mat_table[MatName];
                isLight = mat_table_is_light[MatName];
                isFog = mat_table_is_fog.count(MatName) && mat_table_is_fog[MatName];
            }

            Out::Log(pType::MESSAGE, "Triangle Count = %d", Mesh.triangles_size());
            for(int j=0;j<Mesh.triangles_size();j+=1)
            {
                auto Triangle = Mesh.triangles(j);
                auto A = Triangle.a();
                auto B = Triangle.b();
                auto C = Triangle.c();

                world.add(make_shared<triangle>(vec3(A.x(), A.y(), A.z()), vec3(B.x(), B.y(), B.z()), vec3(C.x(), C.y(), C.z()), ThisMat));

                if(isLight)
                {
                    has_light = true;
                    lights->add(make_shared<triangle>(vec3(A.x(), A.y(), A.z()), vec3(B.x(), B.y(), B.z()), vec3(C.x(), C.y(), C.z()),
                        std::make_shared<material>()));
                }

                if(isFog)
                {
                    has_fog = true;
                }
            }
            
        }

        for(int i=0;i<SceneData->spheres_size();i+=1)
        {
            auto Sphere = SceneData->spheres(i);
            auto MatName = Sphere.matname();

            std::shared_ptr<material> ThisMat = default_mat;
            bool isLight = false;
            bool isFog = false;

            // Check Metarial
            if(mat_table.count(MatName))
            {
                ThisMat = mat_table[MatName];
                isLight = mat_table_is_light[MatName];
                isFog = mat_table_is_fog.count(MatName) && mat_table_is_fog[MatName];
            }

            auto Center = Sphere.center();
            auto Radious = Sphere.radious();

            world.add(make_shared<sphere>(vec3(Center.x(), Center.y(), Center.z()), Radious, ThisMat));

            if(isLight)
            {
                has_light = true;
                lights->add(make_shared<sphere>(vec3(Center.x(), Center.y(), Center.z()), Radious, std::make_shared<material>()));
            }

            if(isFog)
            {
                has_fog = true;
            }
        }
    }

    // auto world = cornell_box();

    color background(0,0,0);

    // Camera

    float camera_location_x = SceneData->cameralocation().x();
    float camera_location_y = SceneData->cameralocation().y();
    float camera_location_z = SceneData->cameralocation().z();
    vec3 location(camera_location_x, camera_location_y, camera_location_z);

    float camera_forward_x = SceneData->cameraforward().x();
    float camera_forward_y = SceneData->cameraforward().y();
    float camera_forward_z = SceneData->cameraforward().z();
    vec3 forward(camera_forward_x, camera_forward_y, camera_forward_z);

    float camera_right_x = SceneData->cameraright().x();
    float camera_right_y = SceneData->cameraright().y();
    float camera_right_z = SceneData->cameraright().z();
    vec3 right(camera_right_x, camera_right_y, camera_right_z);

    point3 lookfrom = location;
    point3 lookat = location + forward;
    vec3 vup = cross(right, forward);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 60.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // Render

    // Hard Code
    if(has_fog)
    {
        samples_per_pixel *= 10240;
    }

    int PixelIndex = 0;

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {

            PixelIndex += 1;

            if(PixelIndex >= Left && PixelIndex <= Right)
            {
                color pixel_color(0,0,0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    if(has_light)
                    {
                        pixel_color += ray_color(r, background, world, lights, max_depth);
                    }
                }

                QVector3D Result(pixel_color.x(), pixel_color.y(), pixel_color.z());
                RenderedPixel(j, i, Result);
            }
        }
    }

    // Out::Log(pType::MESSAGE, "Render Finished");
}