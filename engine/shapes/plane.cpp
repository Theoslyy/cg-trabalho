#include "plane.hpp"
#include "../intersection.hpp"

Plane::Plane () : Shape(), p0(Vec3(0.0, 0.0, 0.0)), normal(Vec3(0.0, -1.0, 0.0)) {}

Plane::Plane (Vec3 p0, Vec3 normal, Material mat) : Shape(mat), p0(p0), normal(normal) {}

Intersection const Plane::get_intersection(Ray r) {
    double top = normal.dot(r.origin - p0);
    double bottom = normal.dot(r.dr);
    if (bottom == 0.0) { return NULL_INTERSECTION; }
    double t = -top/bottom;
    Vec3 p = r.at(t);
    return Intersection(t, p, this->normal, Vec3(1,1,1), this);
}
