
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

olc::Pixel to_pixel (color pixel_color, int samples_per_pixel){
    auto r = pixel_color[0];
    auto g = pixel_color[1];
    auto b = pixel_color[2];

    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return olc::Pixel(
            255 * r,
            255 * g,
            255 * b);
}


#endif
