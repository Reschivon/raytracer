//
// Created by Administrator on 3/24/2021.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

class triangle : public hittable {
public:
    triangle() {}
    triangle(vec3 v0, vec3 v1, vec3 v2, shared_ptr<material> ma) : v0_(v0), v1_(v1), v2_(v2), m(ma) {
        auto t = cross(v1_ - v0_, v2_ - v0_);
        normal = unit_vector(t);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record & rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    vec3 v0_, v1_, v2_;
    shared_ptr<material> m;
    vec3 normal;
};

bool triangle::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(
        vec3(min(v0_.x(), v1_.x(), v2_.x()),
             min(v0_.y(), v1_.y(), v2_.y()),
             min(v0_.z(), v1_.z(), v2_.z())),
        vec3(max(v0_.x(), v1_.x(), v2_.x()),
             max(v0_.y(), v1_.y(), v2_.y()),
             max(v0_.z(), v1_.z(), v2_.z()))
    );
    return true;
}

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    double t, u, v;

    vec3 v0v1 = v1_ - v0_;
    vec3 v0v2 = v2_ - v0_;

    vec3 pvec = cross(r.direction(), v0v2);

    double det = dot(pvec, v0v1);
    double kEpsilon = 0.00001;

    // if the determinant is negative the triangle is backfacing
    // if the determinant is close to 0, the ray misses the triangle

    //if (det < kEpsilon)
    // return false;

    double invDet = 1.0 / det;

    vec3 tvec = r.origin() - v0_;
    u = dot(tvec, pvec) * invDet;

    if (u < 0 || u > 1)
        return false;

    vec3 qvec = cross(tvec, v0v1);
    v = dot(r.direction(), qvec) * invDet;

    if (v < 0 || u + v > 1)
        return false;

    t = dot(v0v2, qvec) * invDet;

    if (t < 0)
        return false;

    if (t > t_max || t < t_min) //object closer than previous
        return false;

    rec.p = r.at(t);
    rec.t = t;

    //rec.set_face_normal(r, vec3(0, 1, 0));
    rec.set_face_normal(r, normal);
    rec.mat_ptr = m;

    return true;
}

#endif //RAYTRACER_TRIANGLE_H
