//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    sphere () {}
    sphere(point3 cen, double r, shared_ptr<material> m)
    : center(cen), radius(r), mat_ptr(m){};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

private:
    static void get_sphere_uv(const point3& p, double& u, double& v) {

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

bool sphere::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(
            center - vec3(radius, radius, radius),
            center + vec3(radius, radius, radius)
            );
    return true;
}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    // quadratic intersection stuff...
    // etc etc
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

    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discr) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    // let hittable make the normal oppose the ray
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}


#endif //RAYTRACER_SPHERE_H
