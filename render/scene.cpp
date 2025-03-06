#include "scene.hpp"
#include <cstddef>
#include <vector>
#include "objects/object.hpp"
#include "light.hpp"
#include "intersection.hpp"

using std::vector;
        
Scene::Scene(): objects(vector<Object*>()), lights(vector<Light*>()), ambient_light(Vec3(1.0,1.0,1.0)) {}
Scene::Scene(Vec3 ambient_light): objects(vector<Object*>()), lights(vector<Light*>()), ambient_light(ambient_light) {}
        
void Scene::add_object(Object* obj) { objects.push_back(obj); }
bool Scene::remove_object(Object* obj) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object* obj2 = objects.at(i);
        if (obj == obj2) {
            objects.erase(objects.begin() + i);
            return true;
        }
    }
    return false;
}
void Scene::remove_object(int n) { objects.erase(objects.begin() + n); }

void Scene::add_light(Light* l) { lights.push_back(l); }
bool Scene::remove_light(Light* l) {
    for (size_t i = 0; i < lights.size(); i++) {
        Light* l2 = lights.at(i);
        if (l == l2) {
            lights.erase(lights.begin() + i);
            return true;
        }
    }
    return false;
}
void Scene::remove_light(int n) { lights.erase(lights.begin() + n); }

Intersection const Scene::get_closest_intersection(Ray ray) {
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
