//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera (double h, double w, double vfov, double aperture, double f_dist, double _time0, double _time1) {
        height = h;
        width = w;
        this->vfov = vfov;

        origin = point3(0, 0, 0);
        lookat = point3(0,0,-1);

        lens_radius = aperture / 2;
        focus_dist = f_dist;

        recalculate();
    }

    void recalculate() {
        auto vup = vec3(0, 1, 0);

        // Camera Settings
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        viewport_width = 2.0 * h;
        viewport_height = viewport_width * height * 1.0 / width;

        w = unit_vector(origin - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // directional vectors
        horizontal = viewport_width * u * focus_dist;
        vertical = viewport_height * v * focus_dist;
        // lower left of rayshooting plane
        lower_left = origin - horizontal/2 - vertical/2 - focus_dist * w;

    }

    ray get_ray(double hori_ratio, double vert_ratio) const {
        vec3 random = lens_radius * random_in_unit_disk();
        vec3 offset = u * random.x() + v * random.y();
        return ray(
                origin + offset,
                lower_left + hori_ratio * horizontal + vert_ratio * vertical - origin - offset,
                random_double(time0, time1));
    }

    point3 origin;
    point3 lookat;
private:
    // the projection plane parameters
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;

    double vfov; //vertical fov

    double viewport_width;
    double viewport_height;
    double lens_radius, focus_dist;

    double time0 = 0;
    double time1 = 0;

    int height;
    int width;
};




#endif //RAYTRACER_CAMERA_H
