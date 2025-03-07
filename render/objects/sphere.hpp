#pragma once
#include "object.hpp"
#include <cmath>
#include "../texture.hpp"

class Sphere : public Object {
    public:
        Vec3 center;
        double radius;
        bool has_texture;
        Texture* texture;
        
        Sphere ();
        Sphere (Vec3 center, double radius, Material mat);
        Sphere (Vec3 center, double radius, Material mat, Texture* texture);

        const Intersection get_intersection(Ray r) override;
        void translate(Vec3 translation_vector) override;
        void transform(TransformationMatrix m) override;
};