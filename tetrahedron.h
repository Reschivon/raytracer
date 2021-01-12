//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_TETRAHEDRON_H
#define RAYTRACER_TETRAHEDRON_H

#include "hittable.h"

class tetrahedron : public hittable {
public:
    tetrahedron () {}

    virtual void distance(const vec3& r, hit_record& rec) const override;

};

void tetrahedron::distance(const vec3& r, hit_record& rec) const {
    vec3 temp_p(r.x(), r.y(), r.z());

    vec3 a1 = vec3(1,1,1);
    vec3 a2 = vec3(-1,-1,1);
    vec3 a3 = vec3(1,-1,-1);
    vec3 a4 = vec3(-1,1,-1);
    vec3 c;
    int n = 0;
    double dist, d;

    double Scale = 2.0;

    while (n < 6) {
        c = a1; dist = (temp_p-a1).length();
        d = (temp_p-a2).length(); if (d < dist) { c = a2; dist=d; }
        d = (temp_p-a3).length(); if (d < dist) { c = a3; dist=d; }
        d = (temp_p-a4).length(); if (d < dist) { c = a4; dist=d; }
        temp_p = (Scale*temp_p)-c*(Scale-1.0);
        n++;
    }

    rec.distance = temp_p.length() * pow(Scale, float(-n));
    rec.contact = r;
}


#endif //RAYTRACER_TETRAHEDRON_H
