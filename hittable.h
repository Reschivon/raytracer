//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "rtweekend.h"

class material;

struct hit_record {
    double distance;
    vec3 contact;
};

class hittable {
public:
    virtual void distance(const vec3& r, hit_record& rec) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
