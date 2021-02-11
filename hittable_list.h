//
// Created by Administrator on 12/11/2020.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include <memory>
#include "hittable.h"
#include "aabb.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) {add(object); }

    void clear() {objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit (
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& object : objects){
        if(object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if(objects.empty())
        return false;

    aabb temp_box;
    bool firstBox = true;

    for(const auto& object : objects){
        object->bounding_box(time0, time1, temp_box);
        output_box = firstBox ? temp_box : surrounding_box(output_box, temp_box);
        firstBox = false;
    }

    return true;
}


#endif //RAYTRACER_HITTABLE_LIST_H
