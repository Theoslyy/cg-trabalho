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
    static TransformationMatrix scale_matrix(double sx, double sy, double sz) {
        return TransformationMatrix({
            {sx, 0.0, 0.0, 0.0},
            {0.0, sy, 0.0, 0.0},
            {0.0, 0.0, sz, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        });
    }

    // Shear matrix along x-axis
    static TransformationMatrix shear_matrix_x(double sh_yz, double sh_zy) {
        return TransformationMatrix({
            {1.0, sh_yz, sh_zy, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        });
    }

    // Shear matrix along y-axis
    static TransformationMatrix shear_matrix_y(double sh_xz, double sh_zx) {
        return TransformationMatrix({
            {1.0, 0.0, 0.0, 0.0},
            {sh_xz, 1.0, sh_zx, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        });
    }

    // Shear matrix along z-axis
    static TransformationMatrix shear_matrix_z(double sh_xy, double sh_yx) {
        return TransformationMatrix({
            {1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {sh_xy, sh_yx, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        });
    }

    // Shear matrix along x-axis based on angle
    static TransformationMatrix shear_matrix_x_angle(double angle) {
        double sh_yz = tan(angle);
        return shear_matrix_x(sh_yz, 0.0);
    }

    // Shear matrix along y-axis based on angle
    static TransformationMatrix shear_matrix_y_angle(double angle) {
        double sh_xz = tan(angle);
        return shear_matrix_y(sh_xz, 0.0);
    }

    // Shear matrix along z-axis based on angle
    static TransformationMatrix shear_matrix_z_angle(double angle) {
        double sh_xy = tan(angle);
        return shear_matrix_z(sh_xy, 0.0);
    }

    // Rotation around an arbitrary axis
    static TransformationMatrix rotation_around_axis(const Vec3& axis, double angle) {
        Vec3 normalized_axis = axis.normalized();
        double x = normalized_axis.x;
        double y = normalized_axis.y;
        double z = normalized_axis.z;
        double cos_theta = cos(angle);
        double sin_theta = sin(angle);
        double one_minus_cos = 1.0 - cos_theta;

        return TransformationMatrix({
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
        });
    }
};

// Define the identity matrix for TransformationMatrix
inline const TransformationMatrix TransformationMatrix::I = TransformationMatrix({
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
});
