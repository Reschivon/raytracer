//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    sphere () {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto a_minus_c = r.origin() - center;
    auto a = r.direction().squared_length();
    auto half_b = dot(r.direction(), a_minus_c);
    auto c = a_minus_c.squared_length() - radius * radius;

    auto discriminant = half_b * half_b - a * c;

    if(discriminant < 0)
        return false;

    auto sqrt_discr = sqrt(discriminant);

    // find the root that lies within range
    auto root = (-half_b - sqrt_discr) / a;
    if (root > t_min || t_max < root) {
        root = (-half_b + sqrt_discr) / a;
        if (root > t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
}


#endif //RAYTRACER_SPHERE_H
