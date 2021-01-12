//
// Created by Administrator on 1/12/2021.
//

#ifndef RAYTRACER_MANDELBULB_H
#define RAYTRACER_MANDELBULB_H

#include "hittable.h"

class mandelbulb : public hittable {
public:
    mandelbulb () {}
    mandelbulb (int iterations, int depth_of_field, int power)
    : iterations(iterations), depth_of_field(depth_of_field), power(power){}

public:
    int iterations = 30;
    int depth_of_field = 2;
    int power = 7;

    virtual void distance(const vec3& r, hit_record& rec) const override;

};

void mandelbulb::distance(const vec3& b, hit_record& rec) const {
    // initialisation
    vec3 z(b.x(), b.y(), b.z());
    double dr = 1; // running derivative
    double r = 0; // escape time length

    // iteration
    int i;
    for (i = 0; i < 30; i++) {
        r = z.length();
        if (r > 2) {
            break;
        }
        // conversion to polar coordinates
        double theta = acos(z.z() / r);
        double phi = atan2(z.y(), z.x());
        dr = pow(r, power - 1.0) * power * dr + 1.0;
        // scaling and rotation
        double zr = pow(r, power);
        theta = theta * power;
        phi = phi * power;
        // conversion back to cartesian coordinates
        z = vec3 (sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
        z = z * zr + b;
    }
    // distance estimator
    rec.distance = 0.5 * log(r) * r / dr;
    rec.contact = b;
}


#endif //RAYTRACER_MANDELBULB_H
