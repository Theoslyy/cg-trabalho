#pragma once
#include <SDL_render.h>
#include "SDL_surface.h"
#include "objects/object.hpp"
#include "scene.hpp"
#include "../math/transform_matrix.hpp"


struct Frame {
    public:
    Vec3 center, dx, dy, pse, p00;
    double width, height;
    int cols, rows;
    
    Frame();
    Frame(Vec3 pos, double width, double height, double cols, double rows);
};

struct Camera {
    enum ProjectionType {
        PERSPECTIVE,
        ORTOGRAPHIC,
        OBLIQUE,
    };
    
    public:
    Vec3 p_eye, bg_color, angle_oblique;
    Vec3 coord_system[3];
    ProjectionType projection_type;
    Frame frame;
    vector<Uint8> pixelBuffer;

    Camera();
    Camera(Vec3 p_eye, double frame_width, double frame_height, double cols, double rows, double frame_distance, Vec3 bg_color);
    
    Vec3 world_to_camera(Vec3 point);
    Vec3 camera_to_world(Vec3 point);
    void draw_scene(SDL_Surface* surface, Scene scene);
    void set_position(Vec3 position);
    void translate(Vec3 traslation_vector);
    void rotate(Vec3 rotation_axis, double angle);
    void transform(TransformationMatrix matrix);

    void look_at(Vec3 point, Vec3 up);
    void set_frame_size(double width, double height);
    void set_frame_distance(double d);
    Intersection send_ray(Scene scene, int x, int y);

private:
    void draw_rows(Scene scene, int start, int end, Vec3 dx, Vec3 dy, Vec3 p00);
};

