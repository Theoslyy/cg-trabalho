#include "cone.hpp"
#include <cmath>
#include "../../math/matrix3.hpp"

Cone::Cone(double r, double h, Vec3 cb, Vec3 dc, Material mat, bool has_base)
    : Object(mat), radius(r), height(h), cb(cb), apex(cb + h * dc.normalized()), dc(dc.normalized()), has_base(has_base) {}

void Cone::translate(Vec3 translation_vector) {
    cb += translation_vector;
    apex += translation_vector;
}

const Intersection Cone::get_intersection(Ray ray) {
    Intersection closest_intersection = Intersection();

    // Check cone surface
    Matrix3 q = dc.projection_matrix();
    Matrix3 m = Matrix3::I - q;
    Vec3 s = ray.origin - cb;

    Vec3 mdr = m * ray.dr;
    Vec3 ms = m * s;
    Vec3 qdr = q * ray.dr;
    Vec3 qs = q * s;
    Vec3 hdc = height * dc;
    double h2 = height * height;
    double r2 = radius * radius;

    double a = h2 * mdr.magnitude_squared() - r2 * qdr.magnitude_squared();
    double b = 2.0 * (h2 * mdr.dot(ms) + r2 * qdr.dot(hdc - qs));
    double c = h2 * ms.magnitude_squared() - r2 * (hdc - qs).magnitude_squared();
    double delta = b * b - 4.0 * a * c;

    if (delta >= 0.0) {
        double t1 = (-b + sqrt(delta)) / (2.0 * a);
        double t2 = (-b - sqrt(delta)) / (2.0 * a);

        // Find the smallest t with a valid intersection on the cone surface
        for (double t : {t1, t2}) {
            if (t >= 0.0 && t < closest_intersection.t) {
                Vec3 cbp = ray.at(t) - cb;
                Vec3 cbe = q * cbp;

                if (cbe.dot(dc) > 0.0 && cbe.magnitude() < height) {
                    Vec3 p = ray.at(t);
                    Vec3 pv = (apex - p).normalized();
                    Matrix3 m_pv = pv.orth_projection_matrix();

                    closest_intersection = Intersection(t, p, (m_pv * dc).normalized(), Vec3(1, 1, 1), this);
                }
            }
        }
    }

    // Check base plane of the cone
    if (has_base) {
        double bottom = ray.dr.dot(-dc);
        if (bottom != 0.0) {
            double t_base = -(ray.origin - cb).dot(-dc) / bottom;

            if (t_base >= 0.0 && t_base < closest_intersection.t && (ray.at(t_base) - cb).magnitude() <= radius) {
                closest_intersection = Intersection(t_base, ray.at(t_base), -dc, Vec3(1, 1, 1), this);
            }
        }
    }

    // Adjust normal direction
    Vec3 n = closest_intersection.normal;
    n = n * -n.dot(ray.dr) / std::abs(n.dot(ray.dr));
    closest_intersection.normal = n;

    return closest_intersection;
}
