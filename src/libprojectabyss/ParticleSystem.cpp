#include "ParticleSystem.h"

void ParticleSystem::addExplosion(const sf::Vector2f& position) {
    for (int i = 0; i < 10; ++i) {
        Particle particle;
        particle.shape.setRadius(2.f);
        particle.shape.setPosition(position);
        particle.shape.setFillColor(sf::Color(255, 200, 0));
        float angle = (rand() % 360) * 3.14159f / 180.f;
        float speed = 2.f + (rand() % 3);
        particle.velocity = sf::Vector2f(cos(angle) * speed, sin(angle) * speed);
        particle.lifetime = 1.0f;
        particles.push_back(particle);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].shape.move(particles[i].velocity);
        particles[i].lifetime -= deltaTime;
        float alpha = (particles[i].lifetime / 1.0f) * 255;
        particles[i].shape.setFillColor(sf::Color(255, 200, 0, static_cast<sf::Uint8>(alpha)));

        if (particles[i].lifetime <= 0) {
            particles.erase(particles.begin() + i);
            --i;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& particle : particles) {
        window.draw(particle.shape);
    }
}