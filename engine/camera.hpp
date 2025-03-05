#pragma once
#include <SDL_render.h>
#include "scene.hpp"

struct Camera {
public:
    Vec3 pos, bg_color;
    Camera();
    Camera(Vec3 pos, double width, double height, double cols, double rows, double viewport_distance, Vec3 bg_color);

    void draw_scene(SDL_Renderer* renderer, Scene scene);

private:
    inline void draw_pixel(SDL_Renderer* renderer, int x, int y, Vec3 color);

    class Viewport {
    public:
        Vec3 pos, dx, dy, top_left, p00;
        double width, height;
        int cols, rows;

        Viewport();
        Viewport(Vec3 pos, double width, double height, double cols, double rows);
    };

    Viewport viewport;
};
