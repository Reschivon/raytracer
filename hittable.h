//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t, u, v;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        // we want the incident ray to oppose the outward_normal
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
