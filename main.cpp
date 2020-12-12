#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster(camera c, double h, double w) : cam(c), height(h), width(w) {
        sAppName = "Example";

    }
    // World
    hittable_list world = random_scene();

    // Image
    const int samples_per_pixel = 2; //1
    const int max_depth = 5; //4
    const int height;
    const int width;
    // Camera
    camera cam;

public:
    bool OnUserCreate() override
    {
//        auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
//        auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
//        auto material_left   = make_shared<dielectric>(1.5);
//        auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

//        world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
//        world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
//        world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
//        world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
//        world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

//        auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
//        auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
//        auto material_left   = make_shared<dielectric>(1.5);
//        auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
//
//        world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
//        world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
//        world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
//        world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
//        world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++) {
            std::cout << "column " << x << " of " << ScreenWidth() << std::endl;
            for (int y = 0; y < ScreenHeight(); y++) {
                color pixel_color(0, 0, 0);
                for(int s = 0; s < samples_per_pixel; s++) {
                    auto hori_ratio = (x + random_double()) / (ScreenWidth() - 1);
                    auto vert_ratio = (y + random_double()) / (ScreenHeight() - 1);

                    ray r = cam.get_ray(hori_ratio, vert_ratio);
                    pixel_color += ray_color(r, world, max_depth);
                }
                Draw(x, ScreenHeight() - 1 - y, to_pixel(pixel_color, samples_per_pixel));
            }
        }
        if(IsFocused()){
            if(GetKey(olc::Key::W).bHeld) {
                cam.origin += vec3(0, 0, -0.1);
            }
            if(GetKey(olc::Key::S).bHeld) {
                cam.origin += vec3(0, 0, 0.1);
            }
            if(GetKey(olc::Key::A).bHeld) {
                cam.origin += vec3(-0.1, 0, 0);
            }
            if(GetKey(olc::Key::D).bHeld) {
                cam.origin += vec3(0.1, 0, 0);
            }
            if(GetKey(olc::Key::SPACE).bHeld) {
                cam.origin += vec3(0, 0.1, 0);
            }
            if(GetKey(olc::Key::SHIFT).bHeld) {
                cam.origin += vec3(0, -0.1, 0);
            }
            cam.recalculate();
        }

        return true;
    }

    color ray_color(const ray& r, const hittable& world, int depth) {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, 0.001, infinity, rec)){
            // metal
            ray scattered;
            color attenuation;
            if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth-1);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t) * color(0.5, 0.7, 1.0) + // light blue
                (t)* color(1.0, 1.0, 1.0);  // white
    }
};


int main()
{
    const int height = 4*120;
    const int width = 4*180;

    raycaster window(camera(height, width, 40.0), height, width);

    window.cam.origin = point3(13,2,3);
    window.cam.lookat = point3(0,0,0);

    window.cam.recalculate();

    if (window.Construct(width, height, 1, 1))
        window.Start();

    return 0;
}

