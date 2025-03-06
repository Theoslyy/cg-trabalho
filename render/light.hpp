#pragma once
#include "../math/vec3.hpp"

struct Light {
    Vec3 pos;
    Vec3 intensity;
    Light (Vec3 pos, Vec3 color, float intensity): pos(pos), intensity(color * intensity) {}
};

// const Light WHITE_LIGHT = Light(Vec3(), Vec3(1.0, 1.0, 1.0), 1.0);