
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

double limit(double min, double max, double val){
    return fmax(fmin(val, max), min);
}

olc::Pixel to_pixel (color pixel_color, int samples_per_pixel){
    auto r = pixel_color[0];
    auto g = pixel_color[1];
    auto b = pixel_color[2];

    auto scale = 1.0 / samples_per_pixel;
    r = scale * r;
    g = scale * g;
    b = scale * b;

    return olc::Pixel(
            limit(0, 255, 255 * r),
            limit(0, 255, 255 * g),
            limit(0, 255, 255 * b));
}



#endif
