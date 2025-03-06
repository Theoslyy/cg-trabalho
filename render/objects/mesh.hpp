#pragma once
#include "object.hpp"
#include <array>
#include <cmath>
#include <string>
#include <vector>
#include "../../math/transform_matrix.hpp"

class Mesh : public Object {
    public:
        vector<Vec3> vertices;
        vector<array<Vec3*, 3>> triangles;
        Material mat;
        Vec3 bounding_box_vmin;
        Vec3 bounding_box_vmax;

        Mesh ();
        Mesh (vector<Vec3> vertices, vector<array<Vec3*, 3>> triangles, Material mat);
        Mesh (string obj_filename);

        const Intersection get_intersection(Ray r) override;
        void translate(Vec3 translation_vector) override;
        void transform(TransformationMatrix m) override;
        static Mesh* cube(Material material);
    private:
        bool intersects_bounding_box(Ray r);
        Intersection intersects_triangle(Ray r, array<Vec3*, 3> triangle);
        void calculate_bounding_box();
        Vec3 triangle_normal(array<Vec3*, 3> triangle);
};