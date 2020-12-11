#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

class raycaster : public olc::PixelGameEngine
{
public:
    raycaster()
    {
        sAppName = "Example";
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        auto focal_length = 1.0;
        auto viewport_width = 2.0;
        auto viewport_height = viewport_width * ScreenHeight() * 1.0 /ScreenWidth();

        // where is the camera
        point3 origin(0,0,0);
        // directional vectors
        point3 horizontal(viewport_width,0,0);
        point3 vertical(0,viewport_height,0);
        // lower left of rayshooting plane
        point3 lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++)
        {
            for (int y = 0; y < ScreenHeight(); y++)
            {
                auto hori_ratio = double(x+1) / ScreenWidth();
                auto vert_ratio = double(y+1) / ScreenHeight();

                // we just want the background, so the ray should not be translated. Subtract origin
                ray r(origin, lower_left + hori_ratio * horizontal + vert_ratio * vertical - origin);

                // std::cout << r.direction() << std::endl;

                color pixel_color = ray_color(r);
                Draw(x, y, to_pixel(pixel_color));
            }
        }
        return true;
    }

    color ray_color(const ray& r) {
        if (hit_sphere(point3(0,0,-1), 0.5, r))
            return color(1, 0, 0);

        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t) * color(0.5, 0.7, 1.0) + // light blue
                    t  * color(1.0, 1.0, 1.0);  // white
    }

    bool hit_sphere(point3 center, double radius, ray r){
        // difference between dot() and *
        // dot does * also but it sums <x y z> into a double
        auto a_minus_c = r.origin() - center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0 * dot(r.direction(), a_minus_c);
        auto c = dot(a_minus_c, a_minus_c) - radius * radius;

        auto discriminant = b * b - 4 * a * c;
        return discriminant > 0;
    }
};




int main()
{
    raycaster demo;
    if (demo.Construct(500, 300, 1, 1))
        demo.Start();

    return 0;
}