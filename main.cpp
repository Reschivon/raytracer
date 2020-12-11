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

        point3 origin(0,0,0);
        point3 horizontal(viewport_width,0,0);
        point3 vertical(0,viewport_height,0);
        point3 upper_left(origin + horizontal/2 + vertical/2 + point3(0,0,focal_length));

        // called once per frame
        for (double x = 0; x < ScreenWidth(); x++)
        {
            for (double y = 0; y < ScreenHeight(); y++)
            {
                auto hori_ratio = (x+1) / ScreenWidth();
                auto vert_ratio = (y+1) / ScreenHeight();
                ray r(origin, upper_left + hori_ratio*horizontal + vert_ratio*vertical);

                color pixel_color = ray_color(r);
                Draw(x, y, to_pixel(pixel_color));
            }
        }
        return true;
    }

    color ray_color(const ray& r) {
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return 250 * ( (1.0-t)*color(1.0, 1.0, 1.0)
            + t*color(0.5, 0.7, 1.0) );
    }
};




int main()
{
    raycaster demo;
    if (demo.Construct(500, 300, 1, 1))
        demo.Start();

    return 0;
}