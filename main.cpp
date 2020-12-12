#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster()
    {
        sAppName = "Example";
    }

    hittable_list world;

public:
    bool OnUserCreate() override
    {

        // World
        world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
        world.add(make_shared<sphere>(point3(0,-100.5,1), 100));

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {

        // Camera
        auto focal_length = 0.7;
        auto viewport_width = 2.0;
        auto viewport_height = viewport_width * ScreenHeight() * 1.0 /ScreenWidth();

        // where is the camera
        auto origin = point3(0, 0, 0);
        // directional vectors
        auto horizontal = vec3(viewport_width, 0, 0);
        auto vertical = vec3(0, viewport_height, 0);
        // lower left of rayshooting plane
        auto lower_left = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++)
        {
            for (int y = 0; y < ScreenHeight(); y++)
            {
                auto hori_ratio = double(x+1) / ScreenWidth();
                auto vert_ratio = double(y+1) / ScreenHeight();

                // we just want the background, so the ray should not be translated. Subtract origin
                ray r(origin, lower_left + hori_ratio * horizontal + vert_ratio * vertical - origin);

                color pixel_color = ray_color(r, world);
                Draw(x, ScreenHeight() - y, to_pixel(pixel_color));
            }
        }
        return true;
    }

    color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, 0, infinity, rec)){
            return 0.5 * (rec.normal + color(1,1,1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return     (1.0-t) * color(0.5, 0.7, 1.0) + // light blue
            (t)* color(1.0, 1.0, 1.0);  // white
    }
};


int main()
{
    raycaster demo;
    if (demo.Construct(500, 300, 1, 1))
        demo.Start();

    return 0;
}