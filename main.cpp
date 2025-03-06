#include <iostream>
#include <cmath>
#include <chrono>
#include <SDL.h>

#include "SDL_keycode.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "math/vec3.hpp"
#include "render/camera.hpp"
#include "render/light.hpp"
#include "render/objects/cilinder.hpp"
#include "render/objects/cone.hpp"
#include "render/scene.hpp"
#include "render/objects/material.hpp"
#include "render/objects/sphere.hpp"
#include "render/objects/plane.hpp"
#include "render/objects/cilinder.hpp"
#include "render/objects/cone.hpp"


using namespace std;

int main() {
    Vec3 p0 = Vec3(0,0,0);
    
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 3.2;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 1.0;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;

    double sphere_radius = 1.0;
    Vec3 sphere_center = Vec3(2.0,0, -(viewport_distance + sphere_radius));

    Vec3 plane_p0 = Vec3(0.0, -1.8, 0.0);
    Vec3 plane_normal = Vec3(0.0, 1.0, 0.0);

    Vec3 plane2_p0 = Vec3(0.0, 0.0, -6.0);
    Vec3 plane2_normal = Vec3(0.0, 0.0, 1.0);
    
    Vec3 bg_color = Vec3(0.0, 0.0, 0.0);
    Material mat_sphere = Material(
        Vec3(0.7, 0.2, 0.2),
        Vec3(0.7, 0.2, 0.2),
        Vec3(0.7, 0.2, 0.2),
        100
    );
    Material mat_p1 = Material(
        Vec3(0.2, 0.7, 0.2),
        Vec3(0.2, 0.7, 0.2),
        Vec3(0.0, 0.0, 0.0),
        1
    );
    Material mat_p2 = Material(
        Vec3(0.3, 0.3, 0.7),
        Vec3(0.3, 0.3, 0.7),
        Vec3(0.0, 0.0, 0.0),
        1
    );

    Sphere* sphere = new Sphere(sphere_center, sphere_radius, mat_sphere);
    Plane* plane = new Plane(plane_p0, plane_normal, mat_p1);
    Plane* plane2 = new Plane(plane2_p0, plane2_normal, mat_p2);
    Cilinder* cilinder = new Cilinder(0.7, 2.0, Vec3(-2.0,-1,-3.0), Vec3(0.0,1.0,1.0), mat_sphere, true, true);
    Cone* cone = new Cone(1.0, 2.0, Vec3(0,-1,-3.0), Vec3(0.0,1.0,0.0), mat_sphere, true);

    Light light = Light(
        Vec3(0.0, 3.8, 2.0),
        Vec3(1.0, 1.0, 1.0),
        0.7
    );

    Vec3 ambient_light = Vec3(0.3, 0.3, 0.3);

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);

    Scene scene = Scene(ambient_light);
    scene.add_object(sphere);
    scene.add_object(cilinder);
    scene.add_object(cone);
    scene.add_object(plane);
    scene.add_object(plane2);

    scene.add_light(&light);

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Trabalho FInal - Computação Gráfica", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    

    // main loop
    SDL_Event event;
    auto fpsTimer = std::chrono::high_resolution_clock::now();
    while (true) {
        // event handler
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) { goto quit; }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        goto quit;
                    case SDLK_w:
                        camera.translate(Vec3::AXIS_Z * -0.1);
                        break;
                    case SDLK_s:
                        camera.translate(Vec3::AXIS_Z *  0.1);
                        break;
                    case SDLK_a:
                        camera.translate(Vec3::AXIS_X * -0.1);
                        break;
                    case SDLK_d:
                        camera.translate(Vec3::AXIS_X *  0.1);
                        break;
                    case SDLK_SPACE:
                        camera.translate(Vec3::AXIS_Y *  0.1);
                        break;
                    case SDLK_LSHIFT:
                        camera.translate(Vec3::AXIS_Y * -0.1);
                        break;
                }
            }
        }
        
        // draw sphere
        auto startTime = std::chrono::high_resolution_clock::now();
        camera.draw_scene(surface, scene);
        SDL_UpdateWindowSurface(window);
        auto endTime = std::chrono::high_resolution_clock::now();

        // printa o FPS no terminal a cada 1s
        auto printTime = endTime - fpsTimer;
        if (printTime >= std::chrono::seconds(1)) {
            std::chrono::duration<double> lastFrameTime = endTime - startTime;
            printf("FPS: %.1f | frame_time: %.2fs\n", 1.0/lastFrameTime.count(), lastFrameTime.count());
            fpsTimer = std::chrono::high_resolution_clock::now(); // reseta o timer
        }
    }
    quit:

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
        