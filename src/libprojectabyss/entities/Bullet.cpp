/**
 * @file Bullet.cpp
 * @brief Реализация класса пули
 */
#include "Bullet.h"
#include <cmath>

Bullet::Bullet(Type bulletType, sf::Vector2f startPos, sf::Vector2f moveDirection, float bulletSpeed)
    : type(bulletType), position(startPos), direction(moveDirection), speed(bulletSpeed), 
      currentState(State::ACTIVE) 
{
    // Нормализация направления
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;
    }
}

void Bullet::update(float deltaTime) {
    if (currentState == State::ACTIVE) {
        position += direction * speed * deltaTime;
    }
}

bool Bullet::isOutOfBounds() const {
    return position.x < 0 || position.x > screenBounds.x || 
           position.y < 0 || position.y > screenBounds.y;
}

void Bullet::setState(State newState) {
    currentState = newState;
}

void Bullet::setPosition(sf::Vector2f pos) {
    position = pos;
}

void Bullet::setDirection(sf::Vector2f dir) {
    // Нормализация направления
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    direction = (length > 0) ? dir / length : sf::Vector2f(0, 0);
}

void Bullet::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Bullet::setType(Type newType) {
    type = newType;
}

void Bullet::setDamage(float newDamage) {
    damage = newDamage;
}

void Bullet::setScreenBounds(sf::Vector2u bounds) {
    screenBounds = bounds;
}

sf::Vector2f Bullet::getPosition() const {
    return position;
}

sf::Vector2f Bullet::getDirection() const {
    return direction;
}

float Bullet::getSpeed() const {
    return speed;
}

Bullet::Type Bullet::getType() const {
    return type;
}

Bullet::State Bullet::getState() const {
    return currentState;
}

float Bullet::getDamage() const {
    return damage;
}

sf::Sprite& Bullet::getSprite() {
    return sprite;
}