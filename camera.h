//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera (double h, double w) {
        height = h;
        width = w;

        // Camera Settings
        focal_length = 0.7;
        viewport_width = 2.0;
        viewport_height = viewport_width * height * 1.0 / width;

        // where is the camera
        origin = point3(0, 0, 0);

        recalculate();
    }

    void recalculate() {
        // directional vectors
        horizontal = vec3(viewport_width, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        // lower left of rayshooting plane
        lower_left = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    }

    ray get_ray(double hori_ratio, double vert_ratio){
        return ray(origin, lower_left + hori_ratio * horizontal + vert_ratio * vertical - origin);
    }

    point3 origin;
private:
    // the projection plane parameters
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;

    double focal_length;
    double viewport_width;
    double viewport_height;

    int height;
    int width;
};




#endif //RAYTRACER_CAMERA_H
