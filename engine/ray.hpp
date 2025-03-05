#pragma once
#include "../utils/vec3.hpp"

struct Ray {
    Vec3 origin;
    Vec3 dr;
    Ray () : origin(Vec3()), dr(Vec3(1.0, 0.0, 0.0)) {};
    Ray (Vec3 origin, Vec3 direction): origin(origin), dr(direction) {};

    inline const Vec3 at(double t) { return origin + dr*t; }
};