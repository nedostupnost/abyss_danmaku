#pragma once
#include "aLibs.h"

class ParticleSystem {
private:
    struct Particle {
        sf::CircleShape shape;
        sf::Vector2f velocity;
        float lifetime;
    };
    std::vector<Particle> particles;

public:
    void addExplosion(const sf::Vector2f& position);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};
