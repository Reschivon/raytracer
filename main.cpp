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
#include "triangle.h"
#include "FastNoiseLite.h"
#include <iostream>
#include <vector>

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

hittable_list night_glassy() {
    hittable_list objects;

    auto ground = make_shared<metal>(color(0.7, 0.4, 0.5), 0.5);
    auto red_plastic = make_shared<metal>(color(1.0, 0.1, 0.1), 0.5);
    auto gold = make_shared<metal>(color(0.9, 0.5, 0.0), 1.2);


    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, gold));
    objects.add(make_shared<sphere>(point3(-2,2,5), 4, red_plastic));

    objects.add(make_shared<sphere>(point3(8,4,3), 2, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(point3(-8,4,3), 3, make_shared<dielectric>(1.5)));

    auto light = make_shared<diffuse_light>(color(5.2, 5.2, 3.2));
    auto light_green = make_shared<diffuse_light>(color(0.7, 2, 0.7));

    objects.add(make_shared<triangle>(
            vec3(-10, 1 ,10),
            vec3(-10, 1, -10),
            vec3(10, 1, 0), gold));

    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, light));
    objects.add(make_shared<sphere>(point3(0,6,0), 1, light_green));
    objects.add(make_shared<sphere>(point3(13,6,0), 1, light));

    return objects;
}

hittable_list terrain(){
    hittable_list objects;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    int w = 20, h = 20;
    std::vector<std::vector<vec3>> elev;
    for(int j = 0; j<w; j++){
        elev.push_back(std::vector<vec3>());
        for(int k = 0; k<h; k++){
            auto x = (j-w/2)*3;
            auto y = (k-h/2)*3;
            elev[j].push_back(vec3(x, noise.GetNoise((float)x * 3, (float)y * 3) * 5, y));
        }
    }
    auto ground = make_shared<lambertian>(color(0.6, 0.7, 0.4));
    auto ground2 = make_shared<lambertian>(color(0.5, 0.7, 0.4));
    for(int j = 0; j<w-1; j++){
        for(int k = 0; k<h-1; k++){
            objects.add(make_shared<triangle>(elev[j][k], elev[j+1][k], elev[j+1][k+1], ground));
            objects.add(make_shared<triangle>(elev[j][k], elev[j][k+1], elev[j+1][k+1], ground2));
        }
    }
    auto light = make_shared<diffuse_light>(color(2.2, 2.2, 2.2));

    auto water = make_shared<metal>(color(0.7, 0.7, 0.9), 1);
//    auto corner0 = point3(-w * 1.5,5.5,-h * 1.5);
//    auto corner1 = point3(w * 1.5,5.5,-h * 1.5);
//    auto corner2 = point3(-w * 1.5,5.5,h * 1.5);
//    auto corner3 = point3(w * 1.5,5.5,h * 1.5);
//    objects.add(make_shared<triangle>(
//            corner0,
//            corner1,
//            corner3,
//            light));
//    objects.add(make_shared<triangle>(
//            corner0,
//            corner2,
//            corner3,
//            water));
    objects.add(make_shared<triangle>(
            point3(-90, -2.5,-30),
            point3(30, -2.5,-30),
            point3(30, -2.5,90),
            water));

//    objects.add(make_shared<triangle>(
//            point3(-31, -1.5,-31),
//            point3(-31, -1.5,31),
//            point3(31, -1.5,31),
//            water));


    objects.add(make_shared<sphere>(point3(0,5,0), 1, light));

    return objects;
}

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster(camera c, double h, double w) : cam(c), height(h), width(w) {
        sAppName = "RayTracer 0.1";
    }
    // World
    // bvh
    bvh_node curr_world = bvh_node(terrain(), 0, 0); //night_glassy(); //random_scene();

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

            auto mult = 1.0;
            if(GetKey(olc::Key::CTRL).bHeld) {
                mult = 5.0;
            }
            if(GetKey(olc::Key::W).bHeld)
                cam.origin -= forward * mult;
            if(GetKey(olc::Key::S).bHeld)
                cam.origin += forward * mult;
            if(GetKey(olc::Key::A).bHeld)
                cam.origin += -right * mult;
            if(GetKey(olc::Key::D).bHeld)
                cam.origin += right * mult;
            if(GetKey(olc::Key::SPACE).bHeld)
                cam.origin += vec3(0, 0.2, 0) * mult;
            if(GetKey(olc::Key::SHIFT).bHeld)
                cam.origin += vec3(0, -0.2, 0) * mult;

            cam.recalculate();

            std::cout << cam.origin << std::endl;
        }

        return true;
    }

    color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        auto unit_r = unit_vector(r.direction());
        if (!world.hit(r, 0.001, infinity, rec)) {
            //if (unit_r.y() > 0)
                return background * 3 * unit_r.z() + vec3(0.4, 0.4, 0.4);// * abs(unit_r.z() - 1);
           // return vec3(0, 0, 0);
        }

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
    const int height = 2*108;
    const int width = 2*192;

    point3 look_from(-82.1527, 39.0637, -53.4905);
    point3 look_at(0,-5,0);

    raycaster window(camera(height, width, 40.0, 0.0, (look_from-look_at).length(), 0, 0), height, width);

    window.samples_per_pixel = 20; //20
    window.max_depth = 19; //10
    //window.background = color(0,0,0);
    window.background = color(0.7, 0.5, 0.2);

    window.cam.origin = look_from;
    window.cam.lookat = look_at;

    window.cam.recalculate();

    if (window.Construct(width, height, 2, 2))
        window.Start();

    return 0;
}

