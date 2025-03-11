#include "pick.hpp"

// Implementação da função pick
Object* pick(Camera& camera, Scene& scene, int mouseX, int mouseY) {
    // Converte as coordenadas do mouse para coordenadas da cena
    double x = static_cast<double>(mouseX);
    double y = static_cast<double>(mouseY);

    // Lança o raio a partir da câmera na direção do clique
    Intersection intersection = camera.send_ray(scene, x, y);

    // Verifica se houve colisão
    if (intersection.t != INFINITY) {
        return intersection.shape; // Retorna o objeto colidido
    }

    return nullptr; // Nenhum objeto foi colidido
}