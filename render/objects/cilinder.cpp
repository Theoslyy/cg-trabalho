#include "cilinder.hpp"
#include "../intersection.hpp"
#include "material.hpp"
#include "../ray.hpp"
#include "../../math/vec3.hpp"
#include "../../math/matrix3.hpp"
#include <cstdio>

Cilinder::Cilinder(double r, double h, Vec3 cb, Vec3 dc, Material mat, bool has_base, bool has_top) 
    : Object(mat), radius(r), height(h), cb(cb), dc(dc.normalized()), ct(cb + h * dc.normalized()), has_base(has_base), has_top(has_top) {}

    
void Cilinder::translate(Vec3 translation_vector) {
    cb += translation_vector;
    ct += translation_vector;
}

void Cilinder::transform(TransformationMatrix m) {
    cb = m * cb;
    ct = m * ct;
    dc = (ct - cb).normalized();
}

Vec3 Cilinder::calculate_center() {
    return (cb + ct) / 2.0; // Ponto médio entre a base e o topo
}

const Intersection Cilinder::get_intersection(Ray ray) {
    Intersection closest_intersection = Intersection();

    // Check superfície cilíndrica
    Matrix3 q = dc.projection_matrix();
    Matrix3 m = Matrix3::I - q;
    Vec3 s = ray.origin - cb;

    Vec3 mdr = m * ray.dr;
    Vec3 ms = m * s;

    double a = mdr.magnitude_squared();
    double b = 2.0 * mdr.dot(ms);
    double c = ms.magnitude_squared() - radius*radius;
    double delta = b*b - 4.0*a*c;

    if (delta >= 0.0) {
        double t1 = (-b + sqrt(delta)) / (2.0 * a);
        double t2 = (-b - sqrt(delta)) / (2.0 * a);

        // Encontra o menor t com uma interseção válida na superfíce cilíndrica
        if (t1 >= 0.0 && t1 < closest_intersection.t) {
            Vec3 cbp = ray.at(t1) - cb;
            Vec3 cbe = q * cbp;
            // retorna só se a interseção está na região válida da superfície
            if (cbe.dot(dc) > 0.0 && cbe.magnitude() < height) {
                closest_intersection = Intersection(t1, ray.at(t1), (m * cbp).normalized(), Vec3(1,1,1), this);
            }
        }

        if (t2 >= 0.0 && t2 < closest_intersection.t) {
            Vec3 cbp = ray.at(t2) - cb;
            Vec3 cbe = q * cbp;
            // retorna só se a interseção está na região válida da superfície
            if (cbe.dot(dc) > 0.0 && cbe.magnitude() < height) {
                closest_intersection = Intersection(t2, ray.at(t2), (m * cbp).normalized(), Vec3(1,1,1), this);
            }
        }
    }

    // Check plano do topo do cilindro
    if (has_top) {
        double bottom = ray.dr.dot(dc);
        if (bottom != 0.0) {
            double t_tampa = -(ray.origin - ct).dot(dc) / bottom;

            if (t_tampa >= 0.0 && t_tampa < closest_intersection.t 
            && (ray.at(t_tampa) - ct).magnitude() <= radius) {
                closest_intersection = Intersection(t_tampa, ray.at(t_tampa), dc, Vec3(1,1,1), this);
            }
        }
    }

    // Check plano da base do cilindro
    if (has_base) {
        double bottom = ray.dr.dot(-dc);
        if (bottom != 0.0) {
            double t_base = -(ray.origin - cb).dot(-dc) / bottom;

            if (t_base >= 0.0 && t_base < closest_intersection.t &&
            (ray.at(t_base) - cb).magnitude() <= radius) {
                closest_intersection = Intersection(t_base, ray.at(t_base), -dc, Vec3(1,1,1), this);
            }
        }
    }

    Vec3 n = closest_intersection.normal;
    n = n * -n.dot(ray.dr) / std::abs(n.dot(ray.dr));
    closest_intersection.normal = n;
    return closest_intersection;
}
