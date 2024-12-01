#pragma once
#include <cmath>
#include "AnimatedSprite.h"
#include "Bullet.h"
#include "Player.h"

// Константы для анимации врага
const sf::IntRect ENEMY_BOUND_IDLE(0, 0, 32, 48);
const std::pair<int, int> ENEMY_SPRITE_DATA_IDLE(0, 4);

// Типы движения врагов
enum EnemyMovementPattern {
    PATTERN_STRAIGHT,    // Прямое движение вниз
    PATTERN_SINE,        // Движение по синусоиде
    PATTERN_ZIGZAG,      // Зигзагообразное движение
    PATTERN_CIRCLE       // Круговое движение
};

// Типы паттернов стрельбы
enum BulletPattern {
    BULLET_SINGLE,     // Одиночный выстрел
    BULLET_CIRCLE,     // Круговой паттерн
    BULLET_SPIRAL,     // Спиральный паттерн
    BULLET_FAN,        // Веерный паттерн
    BULLET_WAVE        // Волновой паттерн
};

// Типы врагов
enum EnemyType {
    ENEMY_BASIC,      // Базовый враг с простой стрельбой
    ENEMY_SPINNER,    // Враг с круговой стрельбой
    ENEMY_SPIRAL,     // Враг со спиральной стрельбой
    ENEMY_SNIPER,     // Враг с веерной стрельбой
    ENEMY_WAVE        // Враг с волновой стрельбой
};

class Enemy {
private:
    AnimatedSprite sprite;
    Vector2f direction;
    float speed;
    float time;         // Время для расчета движения
    EnemyMovementPattern pattern;
    BulletPattern bulletPattern;  // Тип паттерна стрельбы
    float bulletAngle;            // Текущий угол для спирального и кругового паттернов
    float bulletSpeed;            // Скорость пуль
    int bulletsPerShot;          // Количество пуль за один выстрел
    Vector2f startPos;  // Начальная позиция для расчета движения
    EnemyType type;

public:
    int HP;
    int HPMax;
    int damageTimer;
    int shootTimer;
    std::vector<Bullet> bullets;

    Enemy(Texture* texture, Vector2f pos, EnemyMovementPattern pattern = PATTERN_STRAIGHT);
    void update();
    void draw(sf::RenderTarget& target);
    void shoot(Texture* bulletTexture);
    void updateMovement();
    void setType(EnemyType newType);
    void setBulletPattern(BulletPattern pattern);  // Новый метод для установки паттерна стрельбы

    // Методы для доступа к позиции и размерам
    double get_left() { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    sf::Vector2f get_center() { return sprite.get_center(); }
};