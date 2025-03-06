#include "mesh.hpp"
#include "object.hpp"
#include <algorithm>
#include <cmath>
#include "../intersection.hpp"

Mesh::Mesh (): Object(), vertices(), triangles() {};

Mesh::Mesh (vector<Vec3> vertices, vector<array<Vec3*, 3>> triangles, Material mat): Object(mat), vertices(vertices), triangles(triangles) {
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
    // Check if the ray intersects with the bounding box
    if (!intersects_bounding_box(r)) {
        return Intersection(); // No intersection with bounding box
        printf("nao tem na caixa...\n");
    }
    // printf("tem na caixa...\n");

    Intersection closest_intersection;
    double closest_t = INFINITY;

    // Iterate through each triangle in the mesh
    for (const auto& triangle : triangles) {
        Intersection triangle_intersection = intersects_triangle(r, triangle);
        
        // Check if the intersection is valid and closer than the previous closest
        if (triangle_intersection.t > 1e-8 && triangle_intersection.t < closest_t) {
            closest_t = triangle_intersection.t; // Update closest intersection
            closest_intersection = triangle_intersection; // Store the closest intersection
        }
    }

    // If a valid intersection was found, return it
    if (closest_t < INFINITY) {
        return closest_intersection; // Return the closest intersection found
    } else {
        return Intersection(); // Return an empty intersection if none found
    }
}



Vec3 Mesh::triangle_normal(array<Vec3*, 3> triangle) {
    Vec3 edge1 = *triangle[1] - *triangle[0];
    Vec3 edge2 = *triangle[2] - *triangle[0];
    return edge1.cross(edge2).normalized();
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


Intersection Mesh::intersects_triangle(Ray r, std::array<Vec3*, 3> triangle) {
    Vec3 v0 = *triangle[0];
    Vec3 v1 = *triangle[1];
    Vec3 v2 = *triangle[2];

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 h = r.dr.cross(edge2);
    double a = edge1.dot(h);

    if (a > -1e-8 && a < 1e-8) {
        return Intersection(); // Ray is parallel to triangle
    }

    double f = 1.0 / a;
    Vec3 s = r.origin - v0;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0) {
        return Intersection(); // Outside triangle
    }

    Vec3 q = s.cross(edge1);
    double v = f * r.dr.dot(q);

    if (v < 0.0 || u + v > 1.0) {
        return Intersection(); // Outside triangle
    }

    // Calculate t for intersection point
    double t = f * edge2.dot(q);
    if (t >= 0) {
        return Intersection(t, r.at(t), triangle_normal(triangle), Vec3(1, 1, 1), this); // Return intersection
    } else {
        return Intersection(); // Intersection is behind the ray
    }
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
    vector<array<Vec3*, 3>> triangles = {
        // Back face
        {&vertices[2], &vertices[1], &vertices[0]}, {&vertices[1], &vertices[2], &vertices[3]},
        // Left face
        {&vertices[6], &vertices[2], &vertices[0]}, {&vertices[6], &vertices[0], &vertices[4]},
        // Right face
        {&vertices[3], &vertices[5], &vertices[1]}, {&vertices[3], &vertices[7], &vertices[5]},
        // Front face
        {&vertices[4], &vertices[5], &vertices[6]}, {&vertices[7], &vertices[6], &vertices[5]},
        // Top face
        {&vertices[6], &vertices[3], &vertices[2]}, {&vertices[6], &vertices[7], &vertices[3]},
        // Bottom face
        {&vertices[0], &vertices[1], &vertices[5]}, {&vertices[0], &vertices[5], &vertices[4]},
    };

    return new Mesh(vertices, triangles, material);
}
