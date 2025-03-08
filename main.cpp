#include <cstddef>
#include <cmath>
#include <chrono>
#include <SDL.h>

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "math/transform_matrix.hpp"
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
#include "render/texture.hpp"


using namespace std;

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

int main() {
    //posição de camera inicial para estar dentro do quarto da cena:
    Vec3 p0 = Vec3(5,2,8);
    
    //definições gerais da janela
    double aspect_ratio = 16.0/9.0;
    double viewport_width = 6.4;
    double viewport_height = viewport_width/aspect_ratio;
    double viewport_distance = 2.0;
    int image_width = 960;
    int image_height = image_width/aspect_ratio;
    Vec3 bg_color = Vec3(0.0, 0.0, 0.0);
    
    // Definindo os materais a serem usados na cena:
    Material parede_mat(Vec3(0.5, 0.5, 0.5), Vec3(0.6, 0.6, 0.6), Vec3(0.2, 0.2, 0.2), 10.0);
    Material chao_mat(Vec3(0.3, 0.3, 0.3), Vec3(0.5, 0.5, 0.5), Vec3(0.1, 0.1, 0.1), 5.0);
    Material ceu_mat(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 0), 1.0);
    Material arvore_base_mat(Vec3(0.4, 0.2, 0), Vec3(0.5, 0.3, 0.1), Vec3(0.1, 0.1, 0.1), 5.0);
    Material arvore_mat(Vec3(0, 0.5, 0), Vec3(0, 0.6, 0), Vec3(0.1, 0.1, 0.1), 10.0);
    Material bola_verme_mat(Vec3(1, 0, 0), Vec3(1, 0.2, 0.2), Vec3(0.5, 0.5, 0.5), 20.0);
    Material bola_azul_mat(Vec3(0, 0, 1), Vec3(0.2, 0.2, 1), Vec3(0.5, 0.5, 0.5), 20.0);
    Material bola_amar_mat(Vec3(1, 1, 0), Vec3(1, 1, 0.2), Vec3(0.5, 0.5, 0.5), 20.0);

    // Definindo os pontos e material da estrela:
    Material mat_star = Material(
        Vec3(1.0, 0.68, 0.05),
        Vec3(1.0, 0.68, 0.05),
        Vec3(1.0, 0.68, 0.05),
        10
    );

    double x1 = cos(degreesToRadians(18.0));
    double y1 = sin(degreesToRadians(18.0));

    double x2 = cos(degreesToRadians(-54.0));
    double y2 = sin(degreesToRadians(-54.0));

    double x3 = 0.224514; // calculado pelo desmos
    double y3 = 0.30917; // calculado pelo desmos

    double x4 = 0.363271; // calculado pelo desmos
    double y4 = -0.117034; // calculado pelo desmos

    double y5 = -0.381966; // calculado pelo desmos

    vector<Vec3> vertices = {
        Vec3(0,0,0.25), // centro gordinho 1
        Vec3 ( 0.0, 1.0, 0.0 ),
        Vec3 ( x1, y1, 0.0 ),
        Vec3 ( x2, y2, 0.0 ),
        Vec3 ( -x2, y2, 0.0 ),
        Vec3 ( -x1, y1, 0.0 ),
        Vec3 ( 0, 0, -0.25 ), // centro gordinho 2
        Vec3 ( x3, y3, 0), // pentagono interior direita cima
        Vec3 ( x4, y4, 0), // pentagono interior direita baixo
        Vec3 ( 0, y5, 0), // pentagono interior baixo
        Vec3 ( -x4, y4, 0), // pentagono interior esq baixo
        Vec3 ( -x3, y3, 0), // pentagono interior esq cima
    };

    vector<array<size_t, 3>> triangles = {
        {0, 1, 11}, {0, 7, 1}, {0, 2, 7}, {0, 8, 2}, {0, 3, 8}, {0, 9, 3}, {0, 4, 9}, {0, 10, 4}, {0, 5, 10}, {0, 11, 5},
        {6,11, 1}, {6, 1, 7}, {6, 7, 2}, {6, 2, 8}, {6, 8, 3}, {6, 3, 9}, {6, 9, 4}, {6, 4, 10}, {6, 10, 5}, {6, 5, 11}
    };
    
    Mesh* star = new Mesh(vertices, triangles, mat_star);

    // Cubo obsoleto: 
    // Mesh* cube = Mesh::cube(mat_sphere);
    // cube->translate(Vec3(-0.0,-1.0,0));
    // Mesh* cube = new Mesh("teapot400.obj", mat_sphere);
    // cube->transform( // transforma num retangulinho girado
    //     TransformationMatrix::rotation_around_axis(Vec3::AXIS_Y, M_PI/4.0)
    //     * TransformationMatrix::scale_matrix(2.0, 0.5, 0.5) 
    // );

    
    // Definindo algumas luzes:
    Light point_light = Light::point(
        Vec3(5, 8, 5), 
        Vec3(0.6, 0.6, 0.6), 
        1
    );

    Light spotlight = Light::spotlight(
        Vec3(5, 8, 5),
        Vec3(0.0, 1.0, 0.0),
        degreesToRadians(30),
        Vec3(1.0, 1.0, 1.0),
        1.0
    );


    Light directional_light = Light::directional(
        Vec3(0.0, 1.0, 0.0),
        Vec3(1.0, 1.0, 1.0),
        1.0
    );

    Vec3 ambient_light = Vec3(0.2, 0.2, 0.2); // propriedade da cena

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);
    //Definindo cena:
    //TO-DOS: 
    // Ajustar as bolas e a textura tanto dos planos quanto das esferas
    // A arvore ainda está flutuando!
    // Decidir como será a iluminação
    // Um cubo.. presente.. será?
    Scene scene = Scene(ambient_light);
    
     // Estrutura da casa: 
    scene.add_object(new Plane(Vec3(5, 0, 5), Vec3(0, 1, 0), chao_mat)); // chão
    scene.add_object(new Plane(Vec3(0, 2.5, 5), Vec3(1, 0, 0), parede_mat)); // parede esquerda
    scene.add_object(new Plane(Vec3(10, 2.5, 5), Vec3(-1, 0, 0), parede_mat)); // parede direita
    scene.add_object(new Plane(Vec3(5, 2.5, 0), Vec3(0, 0, 1), parede_mat)); // parede de trás
    scene.add_object(new Plane(Vec3(5, 2.5, 10), Vec3(0, 0, -1), parede_mat)); // parede da frente 

    // teto/ceu
    scene.add_object(new Plane(Vec3(5, 10, 5), Vec3(0, -1, 0), ceu_mat));
 
     // arvore de natal
     scene.add_object(new Cilinder(0.5, 1, Vec3(5, 0.5, 5), Vec3(0, 1, 0), arvore_base_mat, true, true)); // tronco da arvore
     scene.add_object(new Cone(1.5, 3, Vec3(5, 1.5, 5), Vec3(0, 1, 0), arvore_mat, true)); // arvore
     
     // bolas!
     scene.add_object(new Sphere(Vec3(4.5, 2, 6.5), 0.2, bola_verme_mat));
     scene.add_object(new Sphere(Vec3(5.5, 2.5, 6.5), 0.2, bola_azul_mat));
     scene.add_object(new Sphere(Vec3(5, 2.2, 6.5), 0.2, bola_amar_mat));
    
    // Aplicando translação na estrela para posiciona-la na arvore
    scene.add_object(star);
    star->translate(Vec3(5, 4.5, 5));
    scene.add_object(star);

    // Luz
    //scene.add_light(&point_light);
    scene.add_light(&spotlight);

    // Olhando para a estrela
    camera.look_at(Vec3(5,4.5,5), Vec3::AXIS_Y);


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
                        camera.set_frame_distance(-camera.frame.center.z - 0.1);
                        break;
                    case SDLK_MINUS:
                        camera.set_frame_distance(-camera.frame.center.z + 0.1);
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
        
