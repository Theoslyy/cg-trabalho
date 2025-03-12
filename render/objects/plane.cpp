#include "plane.hpp"
#include "../intersection.hpp"

Plane::Plane ():
    Object(), p0(Vec3(0.0, 0.0, 0.0)), normal(Vec3(0.0, -1.0, 0.0)), has_texture(false), texture() {}

Plane::Plane (Vec3 p0, Vec3 normal, Material mat):
    Object(mat), p0(p0), normal(normal), has_texture(false), texture(), x_texture_scale(), y_texture_scale() {}

Plane::Plane (Vec3 p0, Vec3 normal, Material mat, Texture* texture, double x_texture_scale, double y_texture_scale):
    Object(mat), p0(p0), normal(normal), has_texture(true), texture(texture), x_texture_scale(x_texture_scale), y_texture_scale(y_texture_scale) {}

void Plane::translate(Vec3 translation_vector) { this->p0 += translation_vector; }

void Plane::transform(TransformationMatrix m) { this->p0 = m * this->p0; }

Vec3 Plane::calculate_center() {
    return p0; // Retorna o ponto de referência do plano como o centro
}

Intersection const Plane::get_intersection(Ray r) {
    double top = normal.dot(r.origin - p0);
    double bottom = normal.dot(r.dr);
    if (bottom == 0.0) { return Intersection(); }
    double t = -top / bottom;
    if (t < 0) return Intersection();
    Vec3 p = r.at(t);

    Vec3 color(1, 1, 1); // Default color
    if (has_texture) {
        // Calculate basis vectors
        Vec3 basis1 = (normal.cross(Vec3(0, 1, 0)));
        if (basis1.magnitude() < 1e-6) { // If the normal is aligned with y-axis, use x-axis
            basis1 = (normal.cross(Vec3(1, 0, 0))); // First basis vector
        }
        basis1 = basis1.normalized();
        Vec3 basis2 = normal.cross(basis1); // Second basis vector

        // Calculate texture coordinates
        double u = (p - p0).dot(basis1) / y_texture_scale; // eixo z
        double v = (p - p0).dot(basis2) / x_texture_scale; // eixo x

        u = u - floor(u); // Wrap around to [0, 1]
        v = v - floor(v); // Wrap around to [0, 1]

        color = texture->sample(u, v);
    }

    return Intersection(t, p, this->normal, color, this);
}

