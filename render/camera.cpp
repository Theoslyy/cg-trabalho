#include "camera.hpp"
#include "intersection.hpp"
#include "objects/object.hpp"
#include <cmath>
#include <cstdio>

Camera::Camera() : p_eye(Vec3()), bg_color(Vec3(1.0, 1.0, 1.0)), frame(Frame()) {}

Camera::Camera(Vec3 p_eye, double frame_width, double frame_height, double cols, double rows, double frame_distance, Vec3 bg_color) :
    p_eye(p_eye), bg_color(bg_color), frame(Frame(Vec3(p_eye.x, p_eye.y, p_eye.z - frame_distance), frame_width, frame_height, cols, rows)) {}

void Camera::draw_scene(SDL_Renderer* renderer, Scene scene) {
    SDL_SetRenderDrawColor(renderer, bg_color.x, bg_color.y, bg_color.z, 1.0);
    SDL_RenderClear(renderer);

    for (int row = 0; row < frame.rows; row++) { // linhas
        for (int col = 0; col < frame.cols; col++ ) { // colunas
            Vec3 dr = ((frame.p00 + frame.dx * col - frame.dy * row) - p_eye).normalized();
            Ray r = Ray(p_eye, dr);
            Intersection closest_intersection = scene.get_closest_intersection(r);

            // checa se a interseção existe e não está atrás da câmera
            if (closest_intersection.t != INFINITY && closest_intersection.t > 0.0) {
                Vec3 i_eye = Vec3(0.0, 0.0, 0.0);
                Vec3 i_ambient = closest_intersection.shape->mat.k_ambient * scene.ambient_light;
                Vec3 i_diffuse;
                Vec3 i_specular;
                Vec3 l, n, r, v;
                double nl, rv;
                
                // Calcula as iluminações
                i_eye += i_ambient;
                for (Light light : scene.lights) {
                    // testa se algum objeto tá entre o observador e a fonte de luz
                    Ray raio_luz = Ray(closest_intersection.p, light.pos - closest_intersection.p);
                    for (Object* obj : scene.objects) {
                        Intersection light_intersection = obj->get_intersection(raio_luz);
                        if (light_intersection.t >= 0.0001 && light_intersection.t <= 0.9999) { goto next_light; }
                    }

                    // calcula a cor pelo modelo de phong
                    l = (light.pos - closest_intersection.p).normalized();
                    n = closest_intersection.normal;
                    r = (2.0 * (l.dot(n)))*n - l;
                    v = -dr;

                    nl = n.dot(l);
                    rv = r.dot(v);
                    if (nl < 0.0) { nl = 0.0; }
                    if (rv < 0.0) { rv = 0.0; }

                    i_diffuse = closest_intersection.shape->mat.k_diffuse * nl * light.intensity * closest_intersection.color;
                    i_specular = closest_intersection.shape->mat.k_specular * pow(rv, closest_intersection.shape->mat.e) * light.intensity * closest_intersection.color;
                    i_eye += i_diffuse + i_specular;

                    next_light:; // goto tag for skipping light calculation
                }

                draw_pixel(renderer, col, row, i_eye.clamp(0.0, 1.0).rgb_255());
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Camera::draw_pixel(SDL_Renderer* renderer, int x, int y, Vec3 color) {
    SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 1.0);
    SDL_RenderDrawPoint(renderer, x, y);
}

Frame::Frame() {
    Vec3 pos = Vec3(0.0, 0.0, -1.0);
    double width = 1.0; double height = 1.0;
    double cols = 256; double rows = 256;

    Vec3 dx = Vec3(width/cols, 0.0, 0.0);
    Vec3 dy = Vec3(0.0, height/cols, 0.0);
    Vec3 pse = Vec3(pos.x - width/2.0, pos.y + height/2.0, pos.z);
    Vec3 p00 = pse + dx/2 - dy/2;

    this->center = pos; this->dx = dx; this->dy = dy; this->pse = pse; this->p00 = p00;
    this->width = width; this->height = height;
    this->cols = cols; this->rows = rows;
}

Frame::Frame(Vec3 pos, double width, double height, double cols, double rows) {
    Vec3 dx = Vec3(width/cols, 0.0, 0.0);
    Vec3 dy = Vec3(0.0, height/rows, 0.0);
    Vec3 pse = Vec3(pos.x - width/2.0, pos.y + height/2.0, pos.z);
    Vec3 p00 = pse + dx/2 - dy/2;

    this->center = pos; this->dx = dx; this->dy = dy; this->pse = pse; this->p00 = p00;
    this->width = width; this->height = height;
    this->cols = cols; this->rows = rows;      
}
