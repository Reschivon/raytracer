//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera (double h, double w, double vfov) {
        height = h;
        width = w;
        this->vfov = vfov;

        origin = point3(0, 0, 0);
        lookat = point3(0,0,-1);

        recalculate();
    }

    void recalculate() {
        auto vup = vec3(0, 1, 0);

        // Camera Settings
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        viewport_width = 2.0 * h;
        viewport_height = viewport_width * height * 1.0 / width;

        auto w = unit_vector(origin - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        // directional vectors
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        // lower left of rayshooting plane
        lower_left = origin - horizontal/2 - vertical/2 - w;

    }

    ray get_ray(double hori_ratio, double vert_ratio) const {
        return ray(origin, lower_left + hori_ratio * horizontal + vert_ratio * vertical - origin);
    }

    point3 origin;
    point3 lookat;
private:
    // the projection plane parameters
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;

    double vfov; //vertical fov

    double viewport_width;
    double viewport_height;

    int height;
    int width;
};




#endif //RAYTRACER_CAMERA_H
