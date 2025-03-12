#pragma once
#include "material.hpp"
#include "../ray.hpp"
#include "../../math/transform_matrix.hpp"

struct Intersection;

class Object {
    public:
        Material mat;
        Object () : mat(Material()) {}
        Object (Material mat) : mat(mat) {}
        
        // Retorna o menor T positivo + informações da colisão do objeto com o raio (INFINITO se não há colisão)
        virtual const Intersection get_intersection(Ray r) = 0;
        virtual ~Object() {} // Declare the destructor as virtual
        virtual void translate(Vec3 translation_vector) = 0;
        virtual void transform(TransformationMatrix m) = 0;
        virtual Vec3 calculate_center() = 0;
};