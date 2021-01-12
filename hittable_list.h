//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include <memory>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) {add(object); }

    void clear() {objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual void distance (const vec3& r, hit_record& re) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

void hittable_list::distance(const vec3 &r, hit_record &rec) const {
    double closest_so_far = std::numeric_limits<double>().max();
    bool has_intersected = false;

    for(const auto& object : objects){
        hit_record temp_record;
        object->distance(r, temp_record);
        if(temp_record.distance > 0) {
            closest_so_far = std::min(closest_so_far, temp_record.distance);
            has_intersected = true;
        }
    }

    rec.distance = closest_so_far;
}


#endif //RAYTRACER_HITTABLE_LIST_H
