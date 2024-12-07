#pragma once
#include <cmath>
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"

// Константы для анимации врага
const sf::IntRect ENEMY_BOUND_IDLE(0, 0, 32, 48);
const std::pair<int, int> ENEMY_SPRITE_DATA_IDLE(0, 4);

// Добавляем количество паттернов для использования в рандомизации
const int NUM_MOVEMENT_PATTERNS = 5;  // Примерное количество паттернов движения
const int NUM_BULLET_PATTERNS = 6;    // Примерное количество паттернов стрельбы

// Типы движения врагов
enum EnemyMovementPattern {
    PATTERN_STRAIGHT,    // Прямое движение вниз
    PATTERN_SINE,        // Движение по синусоиде
    PATTERN_ZIGZAG,      // Зигзагообразное движение
    PATTERN_CIRCLE,      // Круговое движение
    PATTERN_EIGHT        // Движение по восьмерке
};

// Типы паттернов стрельбы
enum BulletPattern {
    BULLET_SINGLE,       // Одиночный выстрел
    BULLET_CIRCLE,       // Круговой паттерн
    BULLET_SPIRAL,       // Спиральный паттерн
    BULLET_FAN,         // Веерный паттерн
    BULLET_WAVE,        // Волновой паттерн
    BULLET_AIMED        // Прицельный выстрел в игрока
};

class Enemy : public Entity {
private:
    Vector2f direction;
    float speed;
    float time;              // Время для расчета движения
    EnemyMovementPattern movementPattern;
    BulletPattern bulletPattern;
    // Удалены неиспользуемые переменные bulletAngle и bulletsPerShot
    float bulletSpeed;       // Скорость пуль
    Vector2f startPos;       // Начальная позиция для расчета движения
    float amplitude;         // Амплитуда для волновых движений
    float frequency;         // Частота для волновых движений

public:
    int damageTimer;
    int shootInterval; // Интервал между выстрелами

    Enemy(Texture* texture, Vector2f pos, 
          EnemyMovementPattern movePattern = PATTERN_STRAIGHT,
          BulletPattern shootPattern = BULLET_SINGLE,
          float enemySpeed = 3.0f,
          int enemyHP = 1);

    void update() override; // Добавляем базовый метод update
    void shoot(Texture* bulletTexture) override; // Добавляем базовый метод shoot
    
    // Специализированные методы для врага
    void update(const Vector2f& playerPos, sf::Texture* bulletTexture);
    void shoot(Texture* bulletTexture, const Vector2f& playerPos);
    void updateMovement();
    void draw(sf::RenderTarget& target) override;
};