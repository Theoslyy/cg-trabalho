#pragma once
#include <cmath>
#include <vector>
#include "objects/object.hpp"
#include "light.hpp"
#include "intersection.hpp"

using std::vector;
using std::tuple;
using std::make_tuple;

struct Scene {
    public:
        vector<Object*> objects;
        vector<Light> lights;
        Vec3 ambient_light;
        
        Scene (): objects(vector<Object*>()), lights(vector<Light>()), ambient_light(Vec3(1.0,1.0,1.0)) {}
        Scene (Vec3 ambient_light): objects(vector<Object*>()), lights(vector<Light>()), ambient_light(ambient_light) {}
        
        void add_object(Object* obj) { objects.push_back(obj); }
        void remove_object(int n) { objects.erase(objects.begin() + n); }

        Intersection const get_closest_intersection(Ray ray) {
            Intersection best_intersection;
            
            // Pega o objeto mais próximo do raio (com t positivo, na frente do raio.)
            for ( Object* s_candidate : this->objects ) {
                Intersection i_candidate = s_candidate->get_intersection(ray);
                if (i_candidate.t >= 0.0 && i_candidate.t < best_intersection.t) {
                    best_intersection = i_candidate;
                }
            }

            return best_intersection;
        }

        void add_light(Light l) { lights.push_back(l); }
        void remove_light(int n) { lights.erase(lights.begin() + n); }
};