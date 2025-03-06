#ifndef CILINDER_HPP
#define CILINDER_HPP

#include "material.hpp"
#include "../ray.hpp"
#include "object.hpp"
#include "../../math/vec3.hpp"

class Cilinder : public Object {
public:
    double radius;
    double height;
    Vec3 cb;
    Vec3 dc;
    Vec3 ct;
    Material material;
    bool has_base;
    bool has_top;

    Cilinder();
    Cilinder(double radius, double height, Vec3 cb, Vec3 dc, Material mat, bool has_base, bool has_top);
    void translate(Vec3 translation_vector);
    const Intersection get_intersection(Ray r);
};

#endif // CILINDER_HPP
