/**
 * @file BulletManager.h
 * @brief Менеджер управления пулями в игре
 */
#pragma once
#include "Bullet.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class BulletManager {
public:
    // Типы менеджеров пуль
    enum class Type {
        PLAYER,
        ENEMY,
        MIXED
    };

private:
    std::vector<std::unique_ptr<Bullet>> bulletPool;
    std::vector<Bullet*> activeBullets;
    const size_t MAX_POOL_SIZE = 200;
    Type managerType = Type::MIXED;
    sf::Vector2u screenBounds;

public:
    // Конструктор с параметрами
    explicit BulletManager(Type type = Type::MIXED, sf::Vector2u bounds = sf::Vector2u(800, 600))
        : managerType(type), screenBounds(bounds) {}

    // Создание пули
    Bullet* createBullet(
        Bullet::Type bulletType, 
        sf::Vector2f position, 
        sf::Vector2f direction, 
        float speed = 500.0f
    );

    // Обновление всех активных пуль
    void update(float deltaTime);

    // Получение активных пуль
    const std::vector<Bullet*>& getActiveBullets() const;

    // Очистка всех пуль
    void clear();

    // Получение текущего количества пуль
    size_t getBulletCount() const;

    // Установка границ экрана
    void setScreenBounds(sf::Vector2u bounds);
};