#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh_node.h"
#include "diffuse_light.h"
#include "xy_rect.h"
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

hittable_list test_scene() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.6, 0.5, 0.8));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    auto material_red = make_shared<metal>(color(0.9, 0.1, 0.2), 0.1);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));

    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));

    world.add(make_shared<sphere>(point3(-1.0, 1.0, -2.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 1.0, -2.0), -0.4, material_left));

    world.add(make_shared<sphere>(point3(-1.0, 1.0, -1.0), 0.5, material_red));
    world.add(make_shared<sphere>(point3(-1.0, 1.0, -1.0), -0.4, material_red));

    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}

hittable_list test_scene2() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    return world;
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<metal>(color(0.7, 0.4, 0.5), 0.5);
    auto pertextAlt = make_shared<metal>(color(1.0, 0.1, 0.1), 0.5);
    auto green = make_shared<metal>(color(0.9, 0.5, 0.0), 1.2);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, pertext));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, green));
    objects.add(make_shared<sphere>(point3(-2,2,5), 4, pertextAlt));

    objects.add(make_shared<sphere>(point3(8,4,3), 2, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(point3(-8,4,3), 3, make_shared<dielectric>(1.5)));

    auto difflight = make_shared<diffuse_light>(color(5.2,5.2,3.2));
    auto difflightBlue = make_shared<diffuse_light>(color(0.7, 2,0.7));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<sphere>(point3(0,6,0), 1, difflightBlue));
    objects.add(make_shared<sphere>(point3(13,6,0), 1, difflight));

    return objects;
}

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster(camera c, double h, double w) : cam(c), height(h), width(w) {
        sAppName = "RayTracer 0.1";
    }
    // World
    hittable_list curr_world = simple_light(); //random_scene();

    // Image
    int samples_per_pixel;
    int max_depth;
    const int height;
    const int width;
    // Camera
    camera cam;

    color background;

public:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++) {
            //std::cout << "column " << x << " of " << ScreenWidth() << std::endl;
            for (int y = 0; y < ScreenHeight(); y++) {
                color pixel_color(0, 0, 0);
                for(int s = 0; s < samples_per_pixel; s++) {
                    auto hori_ratio = (x + random_double()) / (ScreenWidth() - 1);
                    auto vert_ratio = (y + random_double()) / (ScreenHeight() - 1);

                    ray r = cam.get_ray(hori_ratio, vert_ratio);
                    pixel_color += ray_color(r, background, curr_world, max_depth);
                }
                Draw(x, ScreenHeight() - 1 - y, to_pixel(pixel_color, samples_per_pixel));
            }
        }
        if(IsFocused()){
            auto vup = vec3(0, 1, 0);
            auto forward = unit_vector(cam.origin - cam.lookat);
            auto right = unit_vector(cross(vup, forward));

            if(GetKey(olc::Key::W).bHeld) {

                cam.origin -= forward;
            }
            if(GetKey(olc::Key::S).bHeld) {
                cam.origin += forward;
            }
            if(GetKey(olc::Key::A).bHeld) {
                cam.origin += -right;
            }
            if(GetKey(olc::Key::D).bHeld) {
                cam.origin += right;
            }
            cam.origin += right;

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

    color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;
        if (!world.hit(r, 0.001, infinity, rec))
            return background;

        // metal
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted;
        }

        return emitted + attenuation * ray_color(scattered, background, world, depth-1);

        /*vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t) * color(0.5, 0.7, 1.0) + // light blue
                (t)* color(1.0, 1.0, 1.0);  // white*/
    }
};


int main()
{
    const int height = 1.3*108;
    const int width = 1.3*192;

    point3 look_from(26,7,6);
    point3 look_at(0,2,0);

    raycaster window(camera(height, width, 40.0, 0.0, (look_from-look_at).length(), 0, 0), height, width);

    window.samples_per_pixel = 20; //4
    window.max_depth = 10; //25
    window.background = color(0,0,0);
    window.background = color(0.5, 0.7, 1.0);

    window.cam.origin = look_from;
    window.cam.lookat = look_at;

    window.cam.recalculate();

    if (window.Construct(width, height, 2, 2))
        window.Start();

    return 0;
}

