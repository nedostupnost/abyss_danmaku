#include "Bullet.h"

Bullet::Bullet(sf::Texture* texture, sf::Vector2f pos) {
    shape.setRadius(4.f);
    shape.setOrigin(4.f, 4.f); // Устанавливаем центр пули в её середину
    shape.setPosition(pos);
    
    if (texture) {
        shape.setTexture(texture);
    }
    
    velocity = sf::Vector2f(0.f, 5.f); // По умолчанию пуля летит вниз
    rotation = 0.f;
    speed = 5.f;
}

void Bullet::update() {
    // Обновление позиции пули
    shape.move(velocity);
    
    // Обновление вращения пули (если нужно)
    shape.rotate(rotation);
}

void Bullet::setVelocity(float speedX, float speedY) {
    velocity.x = speedX;
    velocity.y = speedY;
}

void Bullet::setVelocity(const sf::Vector2f& vel) {
    velocity = vel;
}

void Bullet::setSpeed(float newSpeed) {
    speed = newSpeed;
    // Обновляем velocity с учетом нового значения скорости
    float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length != 0) {
        velocity = (velocity / length) * speed;
    }
}

void Bullet::setRotation(float rotationSpeed) {
    rotation = rotationSpeed;
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

bool Bullet::isOffscreen(const sf::Vector2u& windowSize) const {
    sf::Vector2f pos = shape.getPosition();
    float radius = shape.getRadius();
    
    return (pos.x + radius < 0 || 
            pos.x - radius > windowSize.x ||
            pos.y + radius < 0 || 
            pos.y - radius > windowSize.y);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}
