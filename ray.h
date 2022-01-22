//
// Created by Administrator on 11/27/2020.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

class ray{
public:
    point3 orig;
    vec3 dir;
    double tm;

    ray() {}
    ray(point3 origin, vec3 direction, double time = 0.0)
        : orig(origin), dir(direction), tm(time)
    {}

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    double time() const    { return tm; }


    point3 at(double t) const {
        return orig + t * dir;
    }

};


#endif //RAYTRACER_RAY_H
