#pragma once
#include "AnimatedSprite.h"
#include "Bullet.h"

// Константы для анимации
const sf::IntRect BOUND_IDLE(32 * 4, 0, 32, 48);
const std::pair<int, int> SPRITE_DATA_IDLE(0, 4);
const sf::IntRect BOUND_FORWARD(0, 0, 32, 48);
const std::pair<int, int> SPRITE_DATA_FORWARD(0, 4);
const sf::IntRect BOUND_LEFT(32 * 4, 48 * 1, 32, 48);
const std::pair<int, int> SPRITE_DATA_LEFT(-4, 4);
const sf::IntRect BOUND_RIGHT(32 * 4, 48 * 2, 32, 48);
const std::pair<int, int> SPRITE_DATA_RIGHT(-4, 4);

const int SPRITE_FRAME_RATE = 6;

class Player {
private:
    AnimatedSprite sprite;
    int animation_state;
    bool movement_direction[4];
    double move_velocity;
    
public:
    int HP;
    int HPMax;
    int shootTimer;
    std::vector<Bullet> bullets;

    Player(Texture* texture, Vector2u windowSize);
    void update();
    void move();
    void check_animation_state();
    void set_move_state(int dir, bool state);
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