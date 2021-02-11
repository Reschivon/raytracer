#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "rtweekend.h"

class aabb {
public:
    point3 minimum, maximum;

    aabb() {}
    aabb(const point3& a, const point3& b) {
        minimum = a;
        maximum = b;
    }

    point3 min() const {
        return minimum;
    }

    point3 max() const{
        return maximum;
    }

    inline bool hit(const ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    /*bool hit(const ray& r, double t_min, double t_max) const {
        for(int a = 0; a < 3; a++) {
            auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                           (maximum[a] - r.origin()[a]) / r.direction()[a]);
            auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                           (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max < t_min)
                return true;
        }
        return false;
    }*/
};

aabb surrounding_box(aabb box0, aabb box1){
    const auto box0min = box0.min();
    const auto box0max = box0.max();
    const auto box1min = box1.min();
    const auto box1max = box1.max();

    point3 small_corner(fmin(box0min.x(), box1min.x()),
                        fmin(box0min.y(), box1min.y()),
                        fmin(box0min.z(), box1min.z()));

    point3 big_corner(fmax(box0max.x(), box1max.x()),
                      fmax(box0max.y(), box1max.y()),
                      fmax(box0max.z(), box1max.z()));

    return aabb(small_corner, big_corner);
}

#endif //RAYTRACER_AABB_H
