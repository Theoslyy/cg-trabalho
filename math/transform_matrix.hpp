#pragma once
#include "vec3.hpp"
#include <initializer_list>

struct TransformationMatrix {
public:
    double m[4][4];
    // Identity matrix
    static const TransformationMatrix I;

    // Constructor with initializer list
    TransformationMatrix(std::initializer_list<std::initializer_list<double>> list) {
        int i = 0;
        for (auto& row : list) {
            int j = 0;
            for (auto& val : row) {
                m[i][j] = val;
                j++;
            }
            i++;
        }
    }

    // Constructor
    TransformationMatrix(double m[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                this->m[i][j] = m[i][j];
            }
        }
    }

    // Default constructor
    TransformationMatrix() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = 0;
            }
        }
    }

    // Addition
    TransformationMatrix operator+(const TransformationMatrix& other) const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    // Subtraction
    TransformationMatrix operator-(const TransformationMatrix& other) const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Multiplication
    TransformationMatrix operator*(const TransformationMatrix& other) const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Matrix * Vec3
    Vec3 operator*(const Vec3& other) const {
        Vec3 result;
        result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + m[0][3];
        result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + m[1][3];
        result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + m[2][3];
        return result;
    }

    // Matrix * n
    TransformationMatrix operator*(double n) const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] * n;
            }
        }
        return result;
    }

    // Matrix / n
    TransformationMatrix operator/(double n) const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[i][j] / n;
            }
        }
        return result;
    }

    // -Matrix
    TransformationMatrix operator-() const {
        TransformationMatrix result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = -m[i][j];
            }
        }
        return result;
    }

    // Overload [] operator
    double* operator[](int index) {
        return m[index];
    }

    const double* operator[](int index) const {
        return m[index];
    }

    // Translation matrix
    static TransformationMatrix translation_matrix(double tx, double ty, double tz) {
        return TransformationMatrix({
            {1.0, 0.0, 0.0, tx},
            {0.0, 1.0, 0.0, ty},
            {0.0, 0.0, 1.0, tz},
            {0.0, 0.0, 0.0, 1.0}
        });
    }

    // Scale matrix
    static TransformationMatrix scale_matrix(double sx, double sy, double sz, Vec3 pivot) {
        return translation_matrix(pivot.x, pivot.y, pivot.z) * TransformationMatrix({
            {sx, 0.0, 0.0, 0.0},
            {0.0, sy, 0.0, 0.0},
            {0.0, 0.0, sz, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        }) * translation_matrix(-pivot.x, -pivot.y, - pivot.z);
    }

    // Shear matrix along x-axis
    static TransformationMatrix shear_matrix_x(double sh_yz, double sh_zy, Vec3 pivot) {
        return translation_matrix(pivot.x, pivot.y, pivot.z) * TransformationMatrix({
            {1.0, sh_yz, sh_zy, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        }) * translation_matrix(-pivot.x, -pivot.y, -pivot.z);
    }

    // Shear matrix along y-axis
    static TransformationMatrix shear_matrix_y(double sh_xz, double sh_zx, Vec3 pivot) {
        return translation_matrix(pivot.x, pivot.y, pivot.z) * TransformationMatrix({
            {1.0, 0.0, 0.0, 0.0},
            {sh_xz, 1.0, sh_zx, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        }) * translation_matrix(-pivot.x, -pivot.y, -pivot.z);
    }

    // Shear matrix along z-axis
    static TransformationMatrix shear_matrix_z(double sh_xy, double sh_yx, Vec3 pivot) {
        return translation_matrix(pivot.x, pivot.y, pivot.z) * TransformationMatrix({
            {1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {sh_xy, sh_yx, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        }) * translation_matrix(-pivot.x, -pivot.y, -pivot.z);
    }

    // Shear matrix along x-axis based on angle
    static TransformationMatrix shear_matrix_x_angle(double angle_yz, double angle_zy, Vec3 pivot) {
        double sh_yz = tan(degreesToRadians(angle_yz));
        double sh_zy = tan(degreesToRadians(angle_zy));
        return shear_matrix_x(sh_yz, sh_zy, pivot);
    }

    // Shear matrix along y-axis based on angle
    static TransformationMatrix shear_matrix_y_angle(double angle_xz, double angle_zx, Vec3 pivot) {
        double sh_xz = tan(degreesToRadians(angle_xz));
        double sh_zx = tan(degreesToRadians(angle_zx));
        return shear_matrix_y(sh_xz, sh_zx, pivot);
    }

    // Shear matrix along z-axis based on angle
    static TransformationMatrix shear_matrix_z_angle(double angle_xy, double angle_yx, Vec3 pivot) {
        double sh_xy = tan(degreesToRadians(angle_xy));
        double sh_yx = tan(degreesToRadians(angle_yx));
        return shear_matrix_z(sh_xy, sh_yx, pivot);
    }

    // Rotation around an arbitrary axis
    static TransformationMatrix rotation_around_axis(const Vec3& axis, double angle, Vec3 pivot) {
        Vec3 normalized_axis = axis.normalized();
        double x = normalized_axis.x;
        double y = normalized_axis.y;
        double z = normalized_axis.z;
        double cos_theta = cos(degreesToRadians(angle));
        double sin_theta = sin(degreesToRadians(angle));
        double one_minus_cos = 1.0 - cos_theta;

        return translation_matrix(pivot.x, pivot.y, pivot.z) * TransformationMatrix({
            {
                cos_theta + x * x * one_minus_cos,
                x * y * one_minus_cos - z * sin_theta,
                x * z * one_minus_cos + y * sin_theta,
                0.0
            },
            {
                y * x * one_minus_cos + z * sin_theta,
                cos_theta + y * y * one_minus_cos,
                y * z * one_minus_cos - x * sin_theta,
                0.0
            },
            {
                z * x * one_minus_cos - y * sin_theta,
                z * y * one_minus_cos + x * sin_theta,
                cos_theta + z * z * one_minus_cos,
                0.0
            },
            {0.0, 0.0, 0.0, 1.0}
        }) * translation_matrix(-pivot.x, -pivot.y, -pivot.z);
    }

    static TransformationMatrix reflection_matrix(const Vec3& pc, const Vec3& normal) {
        Vec3 u = normal.normalized();

        TransformationMatrix householder_matrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j) {
                    householder_matrix.m[i][j] = 1.0;
                } else {
                    householder_matrix.m[i][j] = 0.0;
                }
            }
        }

        householder_matrix.m[0][0] -= 2.0 * u.x * u.x;
        householder_matrix.m[0][1] -= 2.0 * u.x * u.y;
        householder_matrix.m[0][2] -= 2.0 * u.x * u.z;
        householder_matrix.m[1][0] -= 2.0 * u.y * u.x;
        householder_matrix.m[1][1] -= 2.0 * u.y * u.y;
        householder_matrix.m[1][2] -= 2.0 * u.y * u.z;
        householder_matrix.m[2][0] -= 2.0 * u.z * u.x;
        householder_matrix.m[2][1] -= 2.0 * u.z * u.y;
        householder_matrix.m[2][2] -= 2.0 * u.z * u.z;

        return translation_matrix(pc.x, pc.y, pc.z) * householder_matrix * translation_matrix(-pc.x, -pc.y, -pc.z);
    }
    
    private: 
        // Function to convert degrees to radians
        static double degreesToRadians(double degrees) {
            return degrees * (M_PI / 180.0);
        }

        // Function to convert radians to degrees
        static double radiansToDegrees(double radians) {
            return radians * (180.0 / M_PI);
        }
};

inline const TransformationMatrix TransformationMatrix::I = TransformationMatrix({
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
});
