#ifndef PICK_HPP
#define PICK_HPP

#include "scene.hpp"
#include "camera.hpp"
#include "intersection.hpp"

// Função pick: lança um raio e retorna o objeto colidido (se houver)
Object* pick(Camera& camera, Scene& scene, int mouseX, int mouseY);

#endif // PICK_HPP