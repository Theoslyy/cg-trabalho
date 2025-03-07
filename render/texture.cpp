#include "texture.hpp"
#include <stdexcept>

Texture::Texture() {}

Texture::Texture(const std::string& file_name) {
    SDL_Surface* surface = IMG_Load(file_name.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }
    *this = Texture(surface);
    SDL_FreeSurface(surface);
}

Texture::Texture(SDL_Surface* surface) {
    width = surface->w;
    height = surface->h;
    pitch = surface->pitch;
    bpp = pitch / width;

    texture_data = vector<uint8_t>(width * height * bpp);
    memcpy(texture_data.data(), surface->pixels, texture_data.size());
}

Vec3 Texture::sample(double u, double v) const {
    uint32_t x = static_cast<uint32_t>(u * width);
    uint32_t y = static_cast<uint32_t>(v * height);

    size_t index = (y * pitch + x * bpp);
    uint8_t r = texture_data[index];
    uint8_t g = texture_data[index + 1];
    uint8_t b = texture_data[index + 2];

    return Vec3(r / 255.0, g / 255.0, b / 255.0);
}
