#pragma once
#include "object.hpp"
#include <cmath>
#include <string>
#include <array>
#include <vector>
#include "../../math/transform_matrix.hpp"

class Mesh : public Object {
    public:
        vector<Vec3> vertices;
        vector<array<size_t, 3>> triangles;
        Material mat;
        Vec3 bounding_box_vmin;
        Vec3 bounding_box_vmax;

        Mesh ();
        Mesh (vector<Vec3> vertices, vector<array<size_t, 3>> triangles, Material mat);
        Mesh (string obj_filename, Material mat);

        const Intersection get_intersection(Ray r) override;
        void translate(Vec3 translation_vector) override;
        void transform(TransformationMatrix m) override;
        Vec3 calculate_center() override;
        static Mesh* cube(Material material);
    private:
        bool intersects_bounding_box(Ray r);
        Intersection intersects_triangle(Ray r, array<size_t, 3> triangle);
        void calculate_bounding_box();
        Vec3 triangle_normal(array<size_t, 3> triangle);
};