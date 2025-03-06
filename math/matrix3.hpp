#pragma once
#include "vec3.hpp"
#include <initializer_list>

struct Matrix3 {
public:
    double m[3][3];
    // Identity matrix
    static const Matrix3 I;

    // Constructor with initializer list
    Matrix3(std::initializer_list<std::initializer_list<double>> list) {
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
    Matrix3(double m[3][3]) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                this->m[i][j] = m[i][j];
            }
        }
    }

    // Default constructor
    Matrix3() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = 0;
            }
        }
    }

    // Addition
    Matrix3 operator+(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    // Subtraction
    Matrix3 operator-(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // Multiplication
    Matrix3 operator*(const Matrix3& other) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Matrix * Vec3
    Vec3 operator*(const Vec3& other) const {
        Vec3 result;
        result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2];
        result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2];
        result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2];
        return result;
    }

    // Matrix * n
    Matrix3 operator*(double n) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] * n;
            }
        }
        return result;
    }

    // Matrix / n
    Matrix3 operator/(double n) const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = m[i][j] / n;
            }
        }
        return result;
    }

    // -Matrix
    Matrix3 operator-() const {
        Matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
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
};

inline const Matrix3 Matrix3::I = Matrix3({
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}
});
