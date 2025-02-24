#ifndef PLANE_H
#define PLANE_H

#include "shape.h"
#include <cmath>

class Plane : public Shape {
    public:
        Vec3 p0;
        Vec3 normal;
        Plane (): p0(Vec3(0.0, 0.0, 0.0)), normal(Vec3(0.0, -1.0, 0.0)), Shape() {}
        Plane (Vec3 p0, Vec3 normal, Material mat): p0(p0), normal(normal), Shape(mat) {}
        
        inline Vec3 get_normal(Vec3 p) override { return normal; }

        double intersects(Ray r) override {
            double top = normal.dot(r.origin - p0);
            double bottom = normal.dot(r.dr);
            if (bottom == 0.0) { return -INFINITY; }
            return -top/bottom;
        }
};

#endif