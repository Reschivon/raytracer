cmake_minimum_required(VERSION 3.17)
project(raytracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(raytracer main.cpp olcPixelGameEngine.h vec3.h color.h ray.h)