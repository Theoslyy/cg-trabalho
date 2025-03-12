#define SDL_MAIN_HANDLED

#include <cstddef>
#include <cmath>
#include <chrono>
#include <SDL.h>
#include <iostream>
#include <limits>

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
#include "render/pick.hpp"

using namespace std;

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

int main() {
    // Posição de câmera inicial para estar dentro do quarto da cena:
    Vec3 p0 = Vec3(5, 2, 8);
    //Vec3 p0 = Vec3(0, 0, 0);
    // Definições gerais da janela
    double aspect_ratio = 16.0 / 9.0;
    double viewport_width = 6.4;
    double viewport_height = viewport_width / aspect_ratio;
    double viewport_distance = 2.0;
    int image_width = 960;
    int image_height = image_width / aspect_ratio;
    Vec3 bg_color = Vec3(0.0, 0.0, 0.0);

    // Definindo os materiais a serem usados na cena:
    Material arvore_base_mat(Vec3(0.4, 0.2, 0), Vec3(0.5, 0.3, 0.1), Vec3(0.1, 0.1, 0.1), 5.0);
    Material arvore_mat(Vec3(0, 0.5, 0), Vec3(0, 0.6, 0), Vec3(0.1, 0.1, 0.1), 10.0);
    Material mat_white = Material(
        Vec3(0.8, 0.8, 0.8),
        Vec3(0.8, 0.8, 0.8),
        Vec3(0.8, 0.8, 0.8),
        10
    );

    // Texturas:
    Texture vermelho_bola = Texture("red.png");
    Texture amarelo_bola = Texture("gold.png");
    Texture azul_bola = Texture("blue.png");
    Texture ceu = Texture("sky.png");
    Texture chao = Texture("chao.png");
    Texture parede = Texture("wall.png");
    Texture snow = Texture("snow.png");

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
        Vec3(0, 0, 0.25), // centro gordinho 1
        Vec3(0.0, 1.0, 0.0),
        Vec3(x1, y1, 0.0),
        Vec3(x2, y2, 0.0),
        Vec3(-x2, y2, 0.0),
        Vec3(-x1, y1, 0.0),
        Vec3(0, 0, -0.25), // centro gordinho 2
        Vec3(x3, y3, 0), // pentagono interior direita cima
        Vec3(x4, y4, 0), // pentagono interior direita baixo
        Vec3(0, y5, 0), // pentagono interior baixo
        Vec3(-x4, y4, 0), // pentagono interior esq baixo
        Vec3(-x3, y3, 0), // pentagono interior esq cima
    };

    vector<array<size_t, 3>> triangles = {
        {0, 1, 11}, {0, 7, 1}, {0, 2, 7}, {0, 8, 2}, {0, 3, 8}, {0, 9, 3}, {0, 4, 9}, {0, 10, 4}, {0, 5, 10}, {0, 11, 5},
        {6, 11, 1}, {6, 1, 7}, {6, 7, 2}, {6, 2, 8}, {6, 8, 3}, {6, 3, 9}, {6, 9, 4}, {6, 4, 10}, {6, 10, 5}, {6, 5, 11}
    };

    Mesh* star = new Mesh(vertices, triangles, mat_star);

    // Definindo algumas luzes:
    // Light point_light = Light::point(
    //     Vec3(5, 8, 5),
    //     Vec3(0.6, 0.6, 0.6),
    //     1
    // );

    Light spotlight = Light::spotlight(
        Vec3(5, 8, 5),
        Vec3(0.0, 1.0, 0.0),
        degreesToRadians(30),
        Vec3(1.0, 1.0, 1.0),
        1.0
    );

    // Light directional_light = Light::directional(
    //     Vec3(0.0, 1.0, 0.0),
    //     Vec3(1.0, 1.0, 1.0),
    //     1.0
    // );

    Vec3 ambient_light = Vec3(0.2, 0.2, 0.2); // propriedade da cena

    Camera camera = Camera(p0, viewport_width, viewport_height, image_width, image_height, viewport_distance, bg_color);
    Scene scene = Scene(ambient_light);

    // Estrutura da casa:
    scene.add_object(new Plane(Vec3(5, 0, 5), Vec3(0, 1, 0), mat_white, &snow, 4, 4)); // chão
    scene.add_object(new Plane(Vec3(0, 2.5, 5), Vec3(1, 0, 0), mat_white, &parede, 4, 4)); // parede esquerda
    scene.add_object(new Plane(Vec3(10, 2.5, 5), Vec3(-1, 0, 0), mat_white, &parede, 4, 4)); // parede direita
    scene.add_object(new Plane(Vec3(5, 2.5, 0), Vec3(0, 0, 1), mat_white, &parede, 4, 4)); // parede de trás
    scene.add_object(new Plane(Vec3(5, 2.5, 10), Vec3(0, 0, -1), mat_white, &parede, 4, 4)); // parede da frente

    // teto/ceu
    scene.add_object(new Plane(Vec3(5, 10, 5), Vec3(0, -1, 0), mat_white, &ceu, 8, 8));

    // arvore de natal
    scene.add_object(new Cilinder(0.5, 1, Vec3(5, 0., 5), Vec3(0, 1, 0), arvore_base_mat, true, true)); // tronco da arvore
    scene.add_object(new Cone(1.0, 1.5, Vec3(5, 3, 5), Vec3(0, 1, 0), arvore_mat, true)); // arvore
    scene.add_object(new Cone(1.25, 2, Vec3(5, 2, 5), Vec3(0, 1, 0), arvore_mat, true)); // arvore
    scene.add_object(new Cone(1.5, 2.5, Vec3(5, 1, 5), Vec3(0, 1, 0), arvore_mat, true)); // arvore

    // bolas!
    scene.add_object(new Sphere(Vec3(4.5, 1.4, 6.1), 0.2, mat_white, &vermelho_bola));
    scene.add_object(new Sphere(Vec3(5.5, 3, 5.8), 0.2, mat_white, &azul_bola));
    scene.add_object(new Sphere(Vec3(5, 2.2, 6), 0.2, mat_white, &amarelo_bola));

    // Aplicando translação na estrela para posiciona-la na arvore
    scene.add_object(star);
    star->translate(Vec3(5, 4.5, 5));

    // Luz
    scene.add_light(&spotlight);

    // Olhando para a estrela
    camera.look_at(Vec3(5, 4.5, 5), camera.p_eye + Vec3::AXIS_Y);

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Trabalho Final - Computação Gráfica", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // main loop
    SDL_Event event;
    // auto fpsTimer = std::chrono::high_resolution_clock::now();
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
                        camera.translate(camera.coord_system[2] * 0.1);
                        break;
                    case SDLK_a:
                        camera.translate(camera.coord_system[0] * -0.1);
                        break;
                    case SDLK_d:
                        camera.translate(camera.coord_system[0] * 0.1);
                        break;
                    case SDLK_SPACE:
                        camera.translate(Vec3::AXIS_Y * 0.1);
                        break;
                    case SDLK_LSHIFT:
                        camera.translate(Vec3::AXIS_Y * -0.1);
                        break;
                    // Rotation
                    case SDLK_LEFT:
                        camera.rotate(Vec3::AXIS_Y, 2);
                        break;
                    case SDLK_RIGHT:
                        camera.rotate(Vec3::AXIS_Y, -2);
                        break;
                    case SDLK_UP:
                        camera.rotate(camera.coord_system[0], 2);
                        break;
                    case SDLK_DOWN:
                        camera.rotate(camera.coord_system[0], -2);
                        break;
                    case SDLK_q:
                        camera.rotate(camera.coord_system[2], 2);
                        break;
                    case SDLK_e:
                        camera.rotate(camera.coord_system[2], -2);
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
                    case SDLK_l: { //Tecla 'L' para definir o look_at pelo terminal
                        double lx, ly, lz;
                        double ux, uy, uz;
                        printf("COORDENADAS DA CÂMERA: X %.2f | Y %.2f | Z %.2f\n", camera.p_eye.x, camera.p_eye.y, camera.p_eye.z);
                        std::cout << "Digite as coordenadas do look_at (x y z): ";
                        std::cin >> lx >> ly >> lz;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        std::cout << "Digite as coordenadas do up (x y z): ";
                        std::cin >> ux >> uy >> uz;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        camera.look_at(Vec3(lx, ly, lz), Vec3(ux, uy, uz));
                        std::cout << "Look_at definido para (" << lx << ", " << ly << ", " << lz << ")" << std::endl;
                        break;   
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    Intersection i = camera.send_ray(scene, mouseX, mouseY);
                    if (i.t != INFINITY) {
                        camera.look_at(i.p, camera.p_eye + Vec3::AXIS_Y); // olha pro ponto que o usuario clicou
                    }
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    // Usa a função pick para detectar colisão
                    Object* selectedObject = pick(camera, scene, mouseX, mouseY);

                    if (selectedObject != nullptr) {
                        Vec3 c = selectedObject->calculate_center();
                        printf("Centro do objeto selecionado: X %.2f | Y %.2f | Z %.2f\n", c.x, c.y, c.z);

                        // menu de transformações
                        std::cout << "Escolha a transformação" << std::endl;
                        std::cout << "1. Translação" << std::endl;
                        std::cout << "2. Rotação" << std::endl;
                        std::cout << "3. Escala" << std::endl;
                        std::cout << "4. Cisalhamento" << std::endl;
                        std::cout << "5. Cisalhamento (por ângulo)" << std::endl;
                        std::cout << "6. Espelhamento" << std::endl;
                        std::cout << "7. Cancelar" << std::endl;

                        int choice;
                        std::cin >> choice;
                        std::cin.clear(); // Corrige erro de leitura
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        switch (choice) {
                            case 1: { // Translação
                                double tx, ty, tz;
                                std::cout << "Digite os valores de translação (x y z): ";
                                std::cin >> tx >> ty >> tz;
                                selectedObject->translate(Vec3(tx, ty, tz));
                                std::cout << "Translação aplicada!" << std::endl;
                                break;
                            }
                            
                            case 2: { // Rotação
                                double angle;
                                double ax, ay, az;
                                double cx, cy, cz;
                                std::cout << "Digite o eixo de rotação (x y z): ";
                                std::cin >> ax >> ay >> az; 
                                std::cout << "Digite o ângulo de rotação (em graus): ";
                                std::cin >> angle;
                                std::cout << "Digite as coordenadas do pivot (x y z): ";
                                std::cin >> cx >> cy >> cz; 
                                double angleRadians = degreesToRadians(angle);
                                Vec3 pivot = Vec3(cx, cy, cz);
                                TransformationMatrix m = TransformationMatrix::rotation_around_axis(Vec3(ax, ay, az), angleRadians, pivot);
                                selectedObject->transform(m);
                                std::cout << "Rotação aplicada!" << std::endl;
                                break;
                            }
                        
                            case 3: { // Escala
                                double sx, sy, sz;
                                double cx, cy, cz;
                                std::cout << "Digite os valores de escala (x y z): ";
                                std::cin >> sx >> sy >> sz;
                                std::cout << "Digite as coordenadas do pivot (x y z): ";
                                std::cin >> cx >> cy >> cz; 
                                Vec3 pivot = Vec3(cx, cy, cz);
                                TransformationMatrix m = TransformationMatrix::scale_matrix(sx, sy, sz, pivot);
                                selectedObject->transform(m);
                                std::cout << "Escala aplicada!" << std::endl;
                                break;
                            }
                        
                            case 4: { // Cisalhamento
                                int axis;
                                double sh1, sh2;
                                double cx, cy, cz;
                                std::cout << "Escolha o eixo de cisalhamento (1: X, 2: Y, 3: Z): ";
                                std::cin >> axis;
                                std::cout << "Digite os valores de cisalhamento (sh1 sh2): ";
                                std::cin >> sh1 >> sh2;
                                
                                std::cout << "Digite as coordenadas do pivot (x y z): ";
                                std::cin >> cx >> cy >> cz; 
                                Vec3 pivot = Vec3(cx, cy, cz);
                        
                                TransformationMatrix m;
                                switch (axis) {
                                    case 1:
                                        m = TransformationMatrix::shear_matrix_x(sh1, sh2, pivot);
                                        break;
                                    case 2:
                                        m = TransformationMatrix::shear_matrix_y(sh1, sh2, pivot);
                                        break;
                                    case 3:
                                        m = TransformationMatrix::shear_matrix_z(sh1, sh2, pivot);
                                        break;
                                    default:
                                        std::cout << "Eixo inválido!" << std::endl;
                                        break;
                                }
                                selectedObject->transform(m);
                                std::cout << "Cisalhamento aplicado!" << std::endl;
                                break;
                            }

                            case 5: { // Cisalhamento (ângulo)
                                int axis;
                                double sh1, sh2;
                                double cx, cy, cz;
                                std::cout << "Escolha o eixo de cisalhamento (1: X, 2: Y, 3: Z): ";
                                std::cin >> axis;
                                std::cout << "Digite os ângulos de cisalhamento (sh1 sh2): ";
                                std::cin >> sh1 >> sh2;
                                
                                std::cout << "Digite as coordenadas do pivot (x y z): ";
                                std::cin >> cx >> cy >> cz; 
                                Vec3 pivot = Vec3(cx, cy, cz);
                        
                                TransformationMatrix m;
                                switch (axis) {
                                    case 1:
                                        m = TransformationMatrix::shear_matrix_x_angle(sh1, sh2, pivot);
                                        break;
                                    case 2:
                                        m = TransformationMatrix::shear_matrix_y_angle(sh1, sh2, pivot);
                                        break;
                                    case 3:
                                        m = TransformationMatrix::shear_matrix_z_angle(sh1, sh2, pivot);
                                        break;
                                    default:
                                        std::cout << "Eixo inválido!" << std::endl;
                                        break;
                                }
                                selectedObject->transform(m);
                                std::cout << "Cisalhamento aplicado!" << std::endl;
                                break;
                            }
                        
                            case 6: { // Espelhamento
                                double px, py, pz;
                                double cx, cy, cz;
                                std::cout << "Digite o ponto conhecido do espelho (x y z): ";
                                std::cin >> px >> py >> pz;

                                std::cout << "Digite o vetor normal do espelho (x y z): ";
                                std::cin >> cx >> cy >> cz; 
                                Vec3 normal = Vec3(cx, cy, cz);
                        
                                TransformationMatrix m;
                                m = TransformationMatrix::reflection_matrix(Vec3(px, py, pz), normal);
                                selectedObject->transform(m);
                                std::cout << "Espelhamento aplicado!" << std::endl;
                                break;
                            }
                        
                            case 7: // Cancelar
                                std::cout << "Operação cancelada." << std::endl;
                                break;
                        
                            default:
                                std::cout << "Opção inválida!" << std::endl;
                                break;
                        }

                    } else {
                        std::cout << "Nenhum objeto selecionado!" << std::endl;
                    }
                }
            }
        }

        // auto startTime = std::chrono::high_resolution_clock::now();
        camera.draw_scene(surface, scene);
        SDL_UpdateWindowSurface(window);
        // auto endTime = std::chrono::high_resolution_clock::now();

        // printa o FPS no terminal a cada 1s
        // auto printTime = endTime - fpsTimer;
        // if (printTime >= std::chrono::seconds(1)) {
        //     std::chrono::duration<double> lastFrameTime = endTime - startTime;
        //     printf("FPS: %.1f | frame_time: %.2fs\n", 1.0 / lastFrameTime.count(), lastFrameTime.count());
        //     fpsTimer = std::chrono::high_resolution_clock::now(); // reseta o timer
        // }
    }
    quit:
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}