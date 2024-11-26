#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float rotation;  // Скорость вращения пули
    float speed;     // Скорость движения пули

    Bullet(sf::Texture* texture, sf::Vector2f pos);
    
    void update();
    void setVelocity(float speedX, float speedY);
    void setVelocity(const sf::Vector2f& vel);
    void setSpeed(float newSpeed);
    void setRotation(float rotationSpeed);
    
    sf::Vector2f getPosition() const;
    bool isOffscreen(const sf::Vector2u& windowSize) const;
    sf::FloatRect getBounds() const;
};
