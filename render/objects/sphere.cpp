#include "sphere.hpp"
#include "../intersection.hpp"

Sphere::Sphere () : Object(), center(Vec3(0.0, 0.0, 0.0)), radius(1.0) {}

Sphere::Sphere (Vec3 center, double radius, Material mat) : Object(mat), center(center), radius(radius) {}

const Intersection Sphere::get_intersection(Ray r) {
    Vec3 v = center - r.origin;
    double a = r.dr.dot(r.dr);
    double b = -2.0 * (r.dr.dot(v));
    double c = v.dot(v) - radius*radius;
    double delta = b*b - 4*a*c;

    if (delta > 0.0) {
        double t1 = (-b + sqrt(delta)) / (2.0*a);
        double t2 = (-b - sqrt(delta)) / (2.0*a);
        
        double min_t = (t1 < t2 || t2 < 0.0) ? t1 : t2;
        Vec3 p = r.at(min_t);
        Vec3 normal = (p - center).normalized();
        return Intersection(min_t, p, normal, Vec3(1,1,1), this);
    } else {
        return Intersection();
    }
}
