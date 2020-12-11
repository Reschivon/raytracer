
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

olc::Pixel to_pixel (color pixel_color){
    return *new olc::Pixel(pixel_color[0], pixel_color[1], pixel_color[2]);
}

#endif
