//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    sphere () {}
    sphere(point3 cen, double r)
    : center(cen), radius(r) {};

    virtual void distance(const vec3& r, hit_record& rec) const override;

public:
    point3 center;
    double radius;
};

void sphere::distance(const vec3& r, hit_record& rec) const {
    rec.distance = (r - center).length() - radius;
    rec.contact = r;
}


#endif //RAYTRACER_SPHERE_H
