
#ifndef RAYTRACER_DIFFUSE_LIGHT_H
#define RAYTRACER_DIFFUSE_LIGHT_H

class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    shared_ptr<texture> emit;

    virtual bool scatter(
       const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const overide {
        return false;
    }

    virtual color emitted(double u, double v, const point3& p) const override {
        return emit->value(u, v, p);
    }
};

#endif //RAYTRACER_DIFFUSE_LIGHT_H
