#pragma once
#include "../../math/vec3.hpp"

struct Material {
    Vec3 k_ambient;
    Vec3 k_diffuse;
    Vec3 k_specular;
    double e;
    Material (): k_ambient(Vec3()), k_diffuse(Vec3()), k_specular(Vec3()), e(5.0) {};
    Material (Vec3 k_ambient, Vec3 k_diffuse, Vec3 k_specular, double e) : k_ambient(k_ambient), k_diffuse(k_diffuse), k_specular(k_specular), e(e) {};
};
