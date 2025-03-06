#include "camera.hpp"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "intersection.hpp"
#include "objects/object.hpp"
#include <cmath>
#include <cstdio>
#include <vector>
#include <thread>

double clamp(double value, double min, double max) {
    return (value < min) ? min : (value > max) ? max : value;
}

Camera::Camera() : p_eye(Vec3()), bg_color(Vec3(1.0, 1.0, 1.0)), frame(Frame()) {}

Camera::Camera(Vec3 p_eye, double frame_width, double frame_height, double cols, double rows, double frame_distance, Vec3 bg_color) :
    p_eye(p_eye), bg_color(bg_color),
    coord_system{Vec3::AXIS_X, Vec3::AXIS_Y, Vec3::AXIS_Z},
    frame(Frame(Vec3(p_eye.x, p_eye.y, p_eye.z - frame_distance), frame_width, frame_height, cols, rows)),
    pixelBuffer(cols * rows * 4) {}

Vec3 Camera::world_to_camera(Vec3 point) {
    Vec3 point_translated = point - this->p_eye;
    Vec3 point_rotated = Vec3(
        point_translated.dot(this->coord_system[0]),
        point_translated.dot(this->coord_system[1]),
        point_translated.dot(this->coord_system[2])
    );
    return point_rotated;
}

Vec3 Camera::camera_to_world(Vec3 point) {
    Vec3 point_rotated = point.x * this->coord_system[0]
        + point.y * this->coord_system[1]
        + point.z * this->coord_system[2];
    Vec3 point_translated = point_rotated + this->p_eye;
    return point_translated;
}

void Camera::draw_scene(SDL_Surface* surface, Scene scene) {
    Vec3 dx = coord_system[0] * frame.dx.magnitude();
    Vec3 dy = coord_system[1] * frame.dy.magnitude();
    Vec3 p00 = camera_to_world(frame.p00); // TODO: world coords transformation

    vector<std::thread> threads;
    int max_threads = std::thread::hardware_concurrency();
    int rows_per_thread = frame.rows / max_threads;
    
    for (int i = 0; i < max_threads; ++i) {
        int start = i * rows_per_thread;
        int end = (i == max_threads - 1) ? frame.rows : start + rows_per_thread;
        threads.emplace_back(&Camera::draw_rows, this, scene, start, end, dx, dy, p00);
    }

    for (auto& thread : threads) { thread.join(); }
    
    SDL_LockSurface(surface);
    memcpy(surface->pixels, pixelBuffer.data(), pixelBuffer.size());
    SDL_UnlockSurface(surface);
}


void Camera::draw_rows(Scene scene, int start, int end, Vec3 dx, Vec3 dy, Vec3 p00) {
    for (int row = start; row < end; row++) { // linhas
        for (int col = 0; col < frame.cols; col++ ) { // colunas
            Vec3 dr = ((p00 + dx * col - dy * row) - p_eye).normalized();
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
                for (Light* light : scene.lights) {
                    // testa se algum objeto tá entre o observador e a fonte de luz
                    Ray raio_luz = Ray(closest_intersection.p, light->pos - closest_intersection.p);
                    for (Object* obj : scene.objects) {
                        Intersection light_intersection = obj->get_intersection(raio_luz);
                        if (light_intersection.t >= 0.0001 && light_intersection.t <= 0.9999) { goto next_light; }
                    }

                    // calcula a cor pelo modelo de phong
                    l = (light->pos - closest_intersection.p).normalized();
                    n = closest_intersection.normal;
                    r = (2.0 * (l.dot(n)))*n - l;
                    v = -dr;

                    nl = n.dot(l);
                    rv = r.dot(v);
                    if (nl < 0.0) { nl = 0.0; }
                    if (rv < 0.0) { rv = 0.0; }

                    i_diffuse = closest_intersection.shape->mat.k_diffuse * nl * light->intensity * closest_intersection.color;
                    i_specular = closest_intersection.shape->mat.k_specular * pow(rv, closest_intersection.shape->mat.e) * light->intensity * closest_intersection.color;
                    i_eye += i_diffuse + i_specular;

                    next_light:; // goto tag for skipping light calculation
                }

                pixelBuffer[row * frame.cols * 4 + col * 4] = (Uint8) clamp(i_eye.z * 255.0, 0, 255);     // B
                pixelBuffer[row * frame.cols * 4 + col * 4 + 1] = (Uint8) clamp(i_eye.y * 255.0, 0, 255); // G
                pixelBuffer[row * frame.cols * 4 + col * 4 + 2] = (Uint8) clamp(i_eye.x * 255.0, 0, 255); // R
                pixelBuffer[row * frame.cols * 4 + col * 4 + 3] = (Uint8) 255; // A (sempre 255)
                // draw_pixel(renderer, col, row, i_eye.clamp(0.0, 1.0).rgb_255());
            } else {
                pixelBuffer[row * frame.cols * 4 + col * 4] = (Uint8) bg_color.z;     // B
                pixelBuffer[row * frame.cols * 4 + col * 4 + 1] = (Uint8) bg_color.y; // G
                pixelBuffer[row * frame.cols * 4 + col * 4 + 2] = (Uint8) bg_color.x; // R
                pixelBuffer[row * frame.cols * 4 + col * 4 + 3] = (Uint8) 255; // A (sempre 255)
            }
        }
    }
}

void Camera::translate(Vec3 translation_vector) {
    this->p_eye += translation_vector;
}

void Camera::rotate(Vec3 rotation_axis, double angle) {
    TransformationMatrix m = TransformationMatrix::rotation_around_axis(rotation_axis, angle);
    for (int i = 0; i < 3; i++) {
        coord_system[i] = m * coord_system[i];
    }
}

void Camera::transform(TransformationMatrix matrix) {
    for (int i = 0; i < 3; i++) {
        coord_system[i] = matrix * coord_system[i];
    }
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

