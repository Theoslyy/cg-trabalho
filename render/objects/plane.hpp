#pragma once
#include "object.hpp"
#include <cmath>
#include "../texture.hpp"

class Plane : public Object {
    public:
        Vec3 p0;
        Vec3 normal;
        bool has_texture;
        Texture* texture;
        double x_texture_scale;
        double y_texture_scale;

        Plane ();
        Plane (Vec3 p0, Vec3 normal, Material mat);
        Plane (Vec3 p0, Vec3 normal, Material mat, Texture* texture, double x_texture_scale, double y_texture_scale);
        
        const Intersection get_intersection(Ray r) override;
        void translate(Vec3 translation_vector) override;
        void transform(TransformationMatrix m) override;
        Vec3 calculate_center() override;
};
