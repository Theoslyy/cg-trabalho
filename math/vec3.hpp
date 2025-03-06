#pragma once

// #include "matrix3.hpp"
// #include "matrix3.hpp"
#include <cmath>
using namespace std;

inline double clip(double n, double lower, double upper) {
  return std::fmax(lower, std::fmin(n, upper));
}

struct Matrix3;

struct Vec3 {
    double x, y, z;
    Vec3 ():  x(0), y(0), z(0) {};
    Vec3 (double x, double y, double z): x(x), y(y), z(z) {};

    static const Vec3 AXIS_X;
    static const Vec3 AXIS_Y;
    static const Vec3 AXIS_Z;
    static const Vec3 NULL_VEC;
    
    inline Vec3 operator+(const Vec3& other) { return Vec3(x+other.x, y+other.y, z+other.z); }

    inline Vec3 operator-(const Vec3& other) { return Vec3(x-other.x, y-other.y, z-other.z); }

    inline Vec3 operator-() const { return Vec3(-x, -y, -z); }

    inline Vec3 operator*(const double other) const { return Vec3(x*other, y*other, z*other); }

    inline void operator+=(const Vec3& other) { this->x += other.x; this->y += other.y; this->z += other.z; }

    inline friend Vec3 operator*(double scalar, const Vec3& other) { return Vec3(other.x*scalar, other.y*scalar, other.z*scalar); }

    inline Vec3 operator*(const Vec3& other) const { return Vec3(x*other.x, y*other.y, z*other.z); }

    inline Vec3 operator/(const double other) const { return Vec3(x/other, y/other, z/other); }

    inline friend Vec3 operator/(double scalar, const Vec3& other) { return Vec3(other.x/scalar, other.y/scalar, other.z/scalar); }

    inline Vec3 operator/(const Vec3& other) const { return Vec3(x/other.x, y/other.y, z/other.z); }

    inline bool operator==(const Vec3& other) const { return (x == other.x) && (y == other.y) && (z == other.z); }

    inline double dot(const Vec3& other) const { return x*other.x + y*other.y + z*other.z; }

    inline double magnitude() const { return sqrt(x*x + y*y + z*z); }

    inline double magnitude_squared() const { return x*x + y*y + z*z; }

    inline Vec3 normalized() const { return *this / magnitude(); }

    inline Vec3 rgb_normalized() const { return Vec3(x/255.0, y/255.0, z/255.0); }

    inline Vec3 rgb_255() const { return Vec3(x*255.0, y*255.0, z*255.0); }

    inline Vec3 clamp(double min, double max) { return Vec3(clip(x, min, max), clip(y, min, max), clip(z, min, max)); }

    inline Vec3 cross(const Vec3& other) const {
      return Vec3(
          y * other.z - z * other.y, // x-component
          z * other.x - x * other.z, // y-component
          x * other.y - y * other.x  // z-component
      );
    }

    Matrix3 projection_matrix() const;

    Matrix3 orth_projection_matrix() const;
};


inline const Vec3 Vec3::AXIS_X(1, 0, 0);
inline const Vec3 Vec3::AXIS_Y(0, 1, 0);
inline const Vec3 Vec3::AXIS_Z(0, 0, 1);
inline const Vec3 Vec3::NULL_VEC(0, 0, 0);
