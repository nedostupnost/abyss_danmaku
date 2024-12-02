#pragma once
#include <cmath>
#include "AnimatedSprite.h"
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

class Enemy {
private:
    AnimatedSprite sprite;
    Vector2f direction;
    float speed;
    float time;              // Время для расчета движения
    EnemyMovementPattern movementPattern;
    BulletPattern bulletPattern;
    float bulletAngle;       // Текущий угол для спирального и кругового паттернов
    float bulletSpeed;       // Скорость пуль
    int bulletsPerShot;     // Количество пуль за один выстрел
    Vector2f startPos;       // Начальная позиция для расчета движения
    float amplitude;         // Амплитуда для волновых движений
    float frequency;         // Частота для волновых движений

public:
    int HP;
    int HPMax;
    int damageTimer;
    int shootTimer;
    int shootInterval; // Интервал между выстрелами

    std::vector<Bullet> bullets;

    Enemy(Texture* texture, Vector2f pos, 
          EnemyMovementPattern movePattern = PATTERN_STRAIGHT,
          BulletPattern shootPattern = BULLET_SINGLE);

    void update(const Vector2f& playerPos, sf::Texture* bulletTexture);  // Добавляем позицию игрока и текстуру пули для передачи в метод
    void updateMovement();
    void shoot(Texture* bulletTexture, const Vector2f& playerPos);
    void draw(sf::RenderTarget& target);

    // Методы доступа к позиции и размерам
    double get_left() { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    sf::Vector2f get_center() { return sprite.get_center(); }

    // Удаляем неиспользуемые методы
    // void setMovementPattern(EnemyMovementPattern pattern);
    // void setBulletPattern(BulletPattern pattern);
};