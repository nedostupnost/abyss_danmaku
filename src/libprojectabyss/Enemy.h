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
    PATTERN_CIRCLE,      // Круговое движение
    PATTERN_WAVE,        // Волновое появление (как в Touhou)
    PATTERN_SPIRAL       // Спиральное появление
};

// Типы стрельбы врагов
enum EnemyShootPattern {
    SHOOT_SINGLE,        // Одиночный выстрел
    SHOOT_SPREAD,        // Веерная стрельба
    SHOOT_CIRCLE,        // Круговая стрельба
    SHOOT_SPIRAL,        // Спиральная стрельба
    SHOOT_AIMED          // Прицельная стрельба в игрока
};

class Enemy {
private:
    AnimatedSprite sprite;
    sf::Vector2f direction;
    float speed;
    float time;         // Время для расчета движения
    EnemyMovementPattern movePattern;
    EnemyShootPattern shootPattern;
    sf::Vector2f startPos;  // Начальная позиция для расчета движения
    float rotationAngle;  // Угол для спиральной стрельбы
    
public:
    int HP;
    int HPMax;
    int damageTimer;
    int shootTimer;
    std::vector<Bullet> bullets;

    Enemy(sf::Texture* texture, sf::Vector2f pos, EnemyMovementPattern movePattern = PATTERN_STRAIGHT, 
          EnemyShootPattern shootPattern = SHOOT_SINGLE);
    void update();
    void draw(sf::RenderTarget& target);
    void shoot(sf::Texture* bulletTexture, const sf::Vector2f& playerPos);
    void updateMovement();

    // Методы для доступа к позиции и размерам
    double get_left() { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    sf::Vector2f get_center() { return sprite.get_center(); }
};