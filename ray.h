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

    ray() {}
    ray(point3 origin, vec3 direction)
        : orig(origin), dir(direction)
    {}

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t){
        return orig + t * dir;
    }

};


#endif //RAYTRACER_RAY_H
