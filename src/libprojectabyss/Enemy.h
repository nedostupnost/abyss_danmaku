#pragma once
#include "AnimatedSprite.h"
#include "Bullet.h"

// Константы для анимации врага
const sf::IntRect ENEMY_BOUND_IDLE(0, 0, 32, 48);
const std::pair<int, int> ENEMY_SPRITE_DATA_IDLE(0, 4);

class Enemy {
private:
    AnimatedSprite sprite;
    Vector2f direction;
    float speed;
    
public:
    int HP;
    int HPMax;
    int damageTimer;
    std::vector<Bullet> bullets;

    Enemy(Texture* texture, Vector2f pos);
    void update();
    void draw(sf::RenderTarget& target);

    // Добавляем методы для доступа к позиции и размерам
    double get_left() { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    sf::Vector2f get_center() { return sprite.get_center(); }
};