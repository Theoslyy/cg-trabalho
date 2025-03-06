#pragma once
#include "../math/vec3.hpp"
#include <cmath>

struct Light {
    enum LightType {
        Point,
        Spotlight,
        Directional,
    };
    Vec3 pos;
    Vec3 intensity;
    Vec3 dr;
    double angle;
    LightType type;
    
    Light (): pos(0,0,0), intensity(0,0,0), dr(0,0,0), angle( degreesToRadians(45.0) ), type(Point) {}
    
    static Light point(Vec3 pos, Vec3 color, float intensity) {
        Light point_light = Light();
        point_light.pos = pos;
        point_light.intensity = color * intensity;
        point_light.type = Point;
        return point_light;
    }
    
    static Light spotlight(Vec3 pos, Vec3 dr, double angle, Vec3 color, float intensity) {
        Light spotlight = Light();
        spotlight.pos = pos;
        spotlight.dr = dr;
        spotlight.angle = angle;
        spotlight.intensity = color * intensity;
        spotlight.type = Spotlight;
        return spotlight;
    }

    static Light directional(Vec3 dr, Vec3 color, float intensity) {
        Light directional = Light();
        directional.dr = dr;
        directional.intensity = color * intensity;
        directional.type = Directional;
        return directional;
    }
    private:
    double degreesToRadians(double degrees) {
        return degrees * (M_PI / 180.0);
    }
};

// const Light WHITE_LIGHT = Light(Vec3(), Vec3(1.0, 1.0, 1.0), 1.0);