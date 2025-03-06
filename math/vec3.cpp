#include "vec3.hpp"
#include "matrix3.hpp"

// evitar dependencia circular etc bla bla bla eu odeio programação
Matrix3 Vec3::projection_matrix() const {
    return Matrix3(
        {{x * x, x * y, x * z},
         {y * x, y * y, y * z},
         {z * x, z * y, z * z}}
    );
}

Matrix3 Vec3::orth_projection_matrix() const {
    return Matrix3::I - Matrix3(
        {{x * x, x * y, x * z},
         {y * x, y * y, y * z},
         {z * x, z * y, z * z}}
    );
}