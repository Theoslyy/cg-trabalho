#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "../math/vec3.hpp"

class Texture {
public:
    Texture();
    Texture(const std::string& file_name);
    Texture(SDL_Surface* surface);
    Vec3 sample(double u, double v) const;

    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    // Getters for width, height, pitch, and bpp
    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }
    uint32_t getPitch() const { return pitch; }
    uint32_t getBpp() const { return bpp; }

private:
    std::vector<uint8_t> texture_data;
};

#endif // TEXTURE_HPP
