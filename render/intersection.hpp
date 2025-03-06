#pragma once
#include "../math/vec3.hpp"
#include "objects/object.hpp"
#include <cmath>

struct Intersection {
    double t;
    Vec3 p;
    Vec3 normal;
    Vec3 color;
    Object* shape;
    Intersection (): t(INFINITY), p(Vec3(0,0,0)), normal(Vec3(0,0,0)), color(Vec3(0,0,0)), shape(nullptr) {}
    Intersection (double t, Vec3 p, Vec3 normal, Vec3 color, Object* shape): t(t), p(p), normal(normal), color(color), shape(shape) {}
};