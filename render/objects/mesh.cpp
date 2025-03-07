#include "mesh.hpp"
#include "object.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include "../intersection.hpp"

Mesh::Mesh (): Object(), vertices(), triangles() {};

Mesh::Mesh (vector<Vec3> vertices, vector<array<size_t, 3>> triangles, Material mat): Object(mat), vertices(vertices), triangles(triangles) {
    calculate_bounding_box();
};

// Mesh::Mesh (string obj_filename) {

// };

void Mesh::calculate_bounding_box() {
    Vec3 min_v = Vec3(INFINITY, INFINITY, INFINITY);
    Vec3 max_v = Vec3(-INFINITY, -INFINITY, -INFINITY);
    for (Vec3 vertex : vertices) {
        min_v.x = min(vertex.x, min_v.x);
        min_v.y = min(vertex.y, min_v.y);
        min_v.z = min(vertex.z, min_v.z);

        max_v.x = max(vertex.x, max_v.x);
        max_v.y = max(vertex.y, max_v.y);
        max_v.z = max(vertex.z, max_v.z);
    }
    bounding_box_vmax = max_v;
    bounding_box_vmin = min_v;
}

void Mesh::translate(Vec3 translation_vector) {
    bounding_box_vmax += translation_vector;
    bounding_box_vmin += translation_vector;
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i] += translation_vector;
    }
}

void Mesh::transform(TransformationMatrix m) {
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i] = m * vertices[i];
    }
    calculate_bounding_box();
}

const Intersection Mesh::get_intersection(Ray r) {
    if (!intersects_bounding_box(r)) { return Intersection(); }
    Intersection closest_intersection;
    for (array<size_t, 3> triangle : this->triangles) {
        Vec3 tn = triangle_normal(triangle);
        if (tn.dot(r.dr) >= 0.0) { continue; } // backface culling
        Intersection triangle_intersection = intersects_triangle(r, triangle);
        if (triangle_intersection.t < closest_intersection.t) {
            closest_intersection = triangle_intersection;
        }
    }
    return closest_intersection;
}


bool Mesh::intersects_bounding_box(Ray r) {
    double t_min_x = (bounding_box_vmin.x - r.origin.x) / r.dr.x;
    double t_max_x = (bounding_box_vmax.x - r.origin.x) / r.dr.x;
    if (r.dr.x < 0) {
        std::swap(t_min_x, t_max_x); // Swap if ray is going in negative x direction
    }

    double t_min_y = (bounding_box_vmin.y - r.origin.y) / r.dr.y;
    double t_max_y = (bounding_box_vmax.y - r.origin.y) / r.dr.y;
    if (r.dr.y < 0) {
        std::swap(t_min_y, t_max_y); // Swap if ray is going in negative y direction
    }

    double t_min_z = (bounding_box_vmin.z - r.origin.z) / r.dr.z;
    double t_max_z = (bounding_box_vmax.z - r.origin.z) / r.dr.z;
    if (r.dr.z < 0) {
        std::swap(t_min_z, t_max_z); // Swap if ray is going in negative z direction
    }

    // Check for intersection
    if (t_max_x < t_min_y || t_max_y < t_min_x) return false; // No intersection

    double t_min = std::max(t_min_x, std::max(t_min_y, t_min_z));
    double t_max = std::min(t_max_x, std::min(t_max_y, t_max_z));

    return (t_min <= t_max && t_max >= 0); // Return true if there is an intersection
}


Vec3 Mesh::triangle_normal(array<size_t, 3> triangle) {
    Vec3 edge1 = this->vertices[triangle[1]] - this->vertices[triangle[0]];
    Vec3 edge2 = this->vertices[triangle[2]] - this->vertices[triangle[0]];
    return edge1.cross(edge2).normalized();
}


Intersection Mesh::intersects_triangle(Ray r, array<size_t, 3> triangle) {
    // Get the vertices of the triangle
    Vec3 v0 = vertices[triangle[0]];
    Vec3 v1 = vertices[triangle[1]];
    Vec3 v2 = vertices[triangle[2]];

    // Calculate the edges of the triangle
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    // Calculate the determinant
    Vec3 h = r.dr.cross(edge2);
    double a = edge1.dot(h);

    // If the determinant is near zero, the ray lies in the plane of the triangle
    if (a > -1e-8 && a < 1e-8) {
        return Intersection(); // No intersection
    }

    // Calculate the distance from v0 to the ray origin
    Vec3 s = r.origin - v0;
    double u = s.dot(h) / a;

    // Check if the intersection is outside the triangle
    if (u < 0.0 || u > 1.0) {
        return Intersection(); // No intersection
    }

    // Prepare to test the v parameter
    Vec3 q = s.cross(edge1);
    double v = r.dr.dot(q) / a;

    // Check if the intersection is outside the triangle
    if (v < 0.0 || u + v > 1.0) {
        return Intersection(); // No intersection
    }

    // Calculate t to find the intersection point
    double t = edge2.dot(q) / a;

    // If t is positive, the ray intersects the triangle
    if (t > 1e-8) {
        return Intersection(t, r.at(t), triangle_normal(triangle), Vec3(1,1,1), this);
    }

    return Intersection(); // No intersection
}



Mesh* Mesh::cube(Material material) {
    // Vertices of a 1x1x1 cube
    vector<Vec3> vertices = {
        Vec3(-0.5, -0.5, -0.5),
        Vec3( 0.5, -0.5, -0.5),
        Vec3(-0.5,  0.5, -0.5),
        Vec3( 0.5,  0.5, -0.5),
        Vec3(-0.5, -0.5,  0.5),
        Vec3( 0.5, -0.5,  0.5),
        Vec3(-0.5,  0.5,  0.5),
        Vec3( 0.5,  0.5,  0.5),
    };

    // Triangles defined by indices of the vertices
    vector<array<size_t, 3>> triangles = {
        // Back face
        {2, 1, 0}, {1, 2, 3},
        // Left face
        {6, 2, 0}, {6, 0, 4},
        // Right face
        {3, 5, 1}, {3, 7, 5},
        // Front face
        {4, 5, 6}, {7, 6, 5},
        // Top face
        {6, 3, 2}, {6, 7, 3},
        // Bottom face
        {0, 1, 5}, {0, 5, 4},
    };

    return new Mesh(vertices, triangles, material);
}
