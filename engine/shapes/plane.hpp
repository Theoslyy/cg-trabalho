#pragma once
#include "shape.hpp"
#include <cmath>

class Plane : public Shape {
    public:
        Vec3 p0;
        Vec3 normal;
        Plane ();
        Plane (Vec3 p0, Vec3 normal, Material mat);
        
        const Intersection get_intersection(Ray r) override;
};
