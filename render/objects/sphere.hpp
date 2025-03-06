#pragma once
#include "object.hpp"
#include <cmath>

class Sphere : public Object {
    public:
        Vec3 center;
        double radius;
        Sphere ();
        Sphere (Vec3 center, double radius, Material mat);
        
        const Intersection get_intersection(Ray r) override;
};