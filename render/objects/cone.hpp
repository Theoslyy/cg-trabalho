#pragma once

#include "../intersection.hpp"
#include "../ray.hpp"
#include "../../math/vec3.hpp"
#include "material.hpp"
#include "object.hpp"

class Cone : public Object {
public:
    Cone(double r, double h, Vec3 cb, Vec3 dc, Material mat, bool has_base);
    
    const Intersection get_intersection(Ray ray) override;
    void translate(Vec3 translation_vector) override;
    void transform(TransformationMatrix m) override;

private:
    double radius;
    double height;
    Vec3 cb; // Center base
    Vec3 apex; // Apex of the cone
    Vec3 dc; // Direction of the cone
    bool has_base;
};
