#pragma once
#include "AnimatedSprite.h"
#include "Bullet.h"

enum Direction {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

class Player {
private:
    AnimatedSprite sprite;
    bool move_states[4];  // Состояния движения для каждого направления
    sf::Vector2f lastMoveDirection;  // Последнее направление движения для стрельбы

public:
    int HP;
    int HPMax;
    int shootTimer;
    std::vector<Bullet> bullets;

    Player(sf::Texture* texture, sf::Vector2u windowSize);
    void move();
    void update();
    void draw(sf::RenderTarget& target);
    void shoot(sf::Texture* bulletTexture);
    void set_move_state(int dir, bool state);
    sf::Vector2f getShootDirection() const;

    // Методы для доступа к позиции и размерам
    double get_left() const { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    sf::Vector2f get_center() { return sprite.get_center(); }
};