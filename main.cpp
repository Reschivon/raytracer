#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "tetrahedron.h"
#include "mandelbulb.h"
#include "camera.h"
#include "material.h"

#include <iostream>


hittable_list simple_sphere() {
    hittable_list world;
    world.add(make_shared<sphere>(vec3(0,0,-4), 0.5));
    world.add(make_shared<sphere>(vec3(1,1,-7), 0.5));
    world.add(make_shared<sphere>(vec3(2,-1,-10), 0.5));
    return world;
}
hittable_list simple_tet() {
    hittable_list world;
    world.add(make_shared<tetrahedron>());

    return world;
}

hittable_list simple_bub() {
    hittable_list world;
    world.add(make_shared<mandelbulb>());

    return world;
}

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster(camera c, double h, double w) : cam(c), height(h), width(w) {
        sAppName = "Example";

    }
    // World
    hittable_list world = simple_sphere();

    // Image
    const int samples_per_pixel = 2; //1
    const int max_depth = 50; //4
    const int height;
    const int width;
    // Camera
    camera cam;

public:
    bool OnUserCreate() override
    {

        return true;
    }

    color ray_color(const ray& r, const hittable& world, int depth) {
        if (depth <= 0)
            return color(0, 0, 0);

        vec3 sending_dir = unit_vector(r.dir);
        vec3 position = r.orig;
        int iterations = 0;
        hit_record record;

        while (iterations++ < depth){
            world.distance(position, record);
            // hit!
            if(record.distance < 0.01){
                double col_val = flatten(iterations);
                //double col_val = flatten((record.contact - r.orig).length());
                return color(col_val, col_val, col_val);
            }
            // else move to edge of distance field
            position += sending_dir * record.distance;
        }

        return color(0,0,0);
//        vec3 unit_direction = unit_vector(r.direction());
//        auto t = 0.5*(unit_direction.y() + 1.0);
//        return (1.0-t) * color(0.5, 0.7, 1.0) + // light blue
//               (t)* color(1.0, 1.0, 1.0);  // white
    }

    double flatten(double n){
        return pow(n+1, 0.5);
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++)
        {
            //std::cout << "column " << x << " of " << ScreenWidth() << std::endl;
            for (int y = 0; y < ScreenHeight(); y++)
            {
                color pixel_color(0, 0, 0);

                for(int s = 0; s < samples_per_pixel; s++)
                {
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

};


int main()
{
    const int height = 1*120;
    const int width = 1*180;

    raycaster window(camera(height, width, 40.0), height, width);

    window.cam.origin = point3(-0.5,0,3);
    window.cam.lookat = point3(0,0,-4);

    window.cam.recalculate();

    if (window.Construct(width, height, 4, 4))
        window.Start();

    return 0;
}

