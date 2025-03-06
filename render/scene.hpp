#pragma once
#include <vector>
#include "objects/object.hpp"
#include "light.hpp"
#include "intersection.hpp"

using std::vector;

struct Scene {
    public:
        vector<Object*> objects;
        vector<Light*> lights;
        Vec3 ambient_light;
        
        Scene ();
        Scene (Vec3 ambient_light);
        
        void add_object(Object* obj);
        bool remove_object(Object* obj);
        void remove_object(int n);

        void add_light(Light* l);
        bool remove_light(Light* l);
        void remove_light(int n);
        
        Intersection const get_closest_intersection(Ray ray);
};