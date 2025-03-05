#pragma once
#include "material.hpp"
#include "../ray.hpp"

struct Intersection;

class Shape {
    public:
        Material mat;
        Shape () : mat(Material()) {}
        Shape (Material mat) : mat(mat) {}
        
        // Retorna o menor T positivo da colisão do objeto com o raio, 
        // T negativo se não há colisão positiva (objeto atrás do p0 do raio),
        // -INFINITO se não há colisão.
        virtual const Intersection get_intersection(Ray r) = 0;
};