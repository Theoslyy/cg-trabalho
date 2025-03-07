#include <cstddef>
#include <iostream>
#include <cmath>
#include <chrono>
#include <SDL.h>
#include <vector>

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "math/vec3.hpp"
#include "render/camera.hpp"
#include "render/light.hpp"
#include "render/objects/cilinder.hpp"
#include "render/objects/cone.hpp"
#include "render/objects/mesh.hpp"
#include "render/objects/object.hpp"
#include "render/scene.hpp"
#include "render/objects/material.hpp"
#include "render/objects/sphere.hpp"
#include "render/objects/plane.hpp"
#include "render/objects/cilinder.hpp"
#include "render/objects/cone.hpp"


using namespace std;

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

int main() {
    Vec3 p0 = Vec3(0,0,5);
    
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 6.4;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 2.0;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;

    double sphere_radius = 1.0;
    Vec3 sphere_center = Vec3(2.0,0.0, -3.0);

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

    vector<Vec3> vertices = {
        Vec3 (-0.5, 0.1, 0.0),
        Vec3 ( 0.5, 0.1, 0.0),
        Vec3 (0.0, 1.0, 0.0)
    };

    vector<array<size_t, 3>> triangles = {
        {0, 1, 2}
    };

    // Mesh* cube = new Mesh(vertices, triangles, mat_sphere);
    Mesh* cube = Mesh::cube(mat_sphere);
    // cube->translate(sphere_center);

    Light point_light = Light::point(
        Vec3(0.0, 3.8, 2.0),
        Vec3(1.0, 1.0, 1.0),
        0.7
    );

    Light spotlight = Light::spotlight(
        Vec3(0.0, 3.8, -3.0),
        Vec3(0.0, 1.0, 0.0),
        degreesToRadians(45),
        Vec3(1.0, 1.0, 1.0),
        0.7
    );


    Light directional_light = Light::directional(
        Vec3(0.0, 1.0, 0.0),
        Vec3(1.0, 1.0, 1.0),
        0.7
    );

    Vec3 ambient_light = Vec3(0.3, 0.3, 0.3); // propriedade da cena

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);

    Scene scene = Scene(ambient_light);
    scene.add_object(sphere);
    scene.add_object(cube);
    scene.add_object(cilinder);
    scene.add_object(cone);
    scene.add_object(plane);
    scene.add_object(plane2);

    scene.add_light(&point_light);

    // camera.look_at(sphere_center, Vec3::AXIS_Y);

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Trabalho FInal - Computação Gráfica", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // main loop
    SDL_Event event;
    auto fpsTimer = std::chrono::high_resolution_clock::now();
    while (true) {
        // event handler
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                goto quit;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        goto quit;
                    // Movement
                    case SDLK_w:
                        camera.translate(camera.coord_system[2] * -0.1);
                        break;
                    case SDLK_s:
                        camera.translate(camera.coord_system[2] *  0.1);
                        break;
                    case SDLK_a:
                        camera.translate(camera.coord_system[0] * -0.1);
                        break;
                    case SDLK_d:
                        camera.translate(camera.coord_system[0] *  0.1);
                        break;
                    case SDLK_SPACE:
                        camera.translate(Vec3::AXIS_Y *  0.1);
                        break;
                    case SDLK_LSHIFT:
                        camera.translate(Vec3::AXIS_Y * -0.1);
                        break;
                    // Rotation
                    case SDLK_LEFT:
                        camera.rotate(Vec3::AXIS_Y, 0.1);
                        break;
                    case SDLK_RIGHT:
                        camera.rotate(Vec3::AXIS_Y, -0.1);
                        break;
                    case SDLK_UP:
                        camera.rotate(camera.coord_system[0], 0.1);
                        break;
                    case SDLK_DOWN:
                        camera.rotate(camera.coord_system[0], -0.1);
                        break;
                    case SDLK_q:
                        camera.rotate(camera.coord_system[2], 0.1);
                        break;
                    case SDLK_e:
                        camera.rotate(camera.coord_system[2], -0.1);
                        break;
                    // FOV / distancia focal
                    case SDLK_EQUALS:
                        camera.set_frame_distance(-camera.frame.center.z - 0.01);
                        break;
                    case SDLK_MINUS:
                        camera.set_frame_distance(-camera.frame.center.z + 0.01);
                        break;
                    // Projeções
                    case SDLK_1:
                        camera.projection_type = Camera::PERSPECTIVE; // muda a projeção pra perspectiva
                        break;
                    case SDLK_2:
                        camera.projection_type = Camera::ORTOGRAPHIC; // muda a projeção pra ortografica
                        break;
                    case SDLK_3:
                        camera.projection_type = Camera::OBLIQUE;     // muda a projeção pra obliqua
                        break;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                Intersection i = camera.send_ray(scene, x, y);
                if (i.t != INFINITY) {
                    camera.look_at(i.p, Vec3::AXIS_Y); // olha pro ponto que o usuario clicou
                    // scene.add_object(new Sphere(i.p, 0.25, mat_sphere)); // adiciona uma esfera onde o raio bateu
                }
            }
        }
        
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
        