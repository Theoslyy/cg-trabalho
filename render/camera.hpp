#pragma once
#include <SDL_render.h>
#include "scene.hpp"

enum ProjectionType {
    PERSPECTIVE,
    ORTOGRAPHIC,
    OBLIQUE,
};

struct Frame {
public:
    Vec3 center, dx, dy, pse, p00;
    double width, height;
    int cols, rows;

    Frame();
    Frame(Vec3 pos, double width, double height, double cols, double rows);
};

struct Camera {
public:
    Vec3 p_eye, bg_color, angle_oblique;
    Vec3 coord_system[3];
    double frame_distance;
    ProjectionType projection_type;
    Frame frame;

    Camera();
    Camera(Vec3 p_eye, double frame_width, double frame_height, double cols, double rows, double frame_distance, Vec3 bg_color);

    void draw_scene(SDL_Renderer* renderer, Scene scene);
    void set_position(Vec3 position);
    void translate(Vec3 traslation_vector);
    void rotate(Vec3 rotation_axis, double angle);
private:
    inline void draw_pixel(SDL_Renderer* renderer, int x, int y, Vec3 color);
};

