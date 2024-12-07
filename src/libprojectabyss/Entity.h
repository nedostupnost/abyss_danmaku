#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Bullet.h"
#include "BoundedSprite.h"

using sf::Vector2f;
using sf::Texture;

class Entity {
protected:
    AnimatedSprite sprite;
    std::vector<Bullet> bullets;
    int HP;
    int HPMax;
    int shootTimer;

public:
    Entity(Texture* texture, Vector2f position);
    virtual ~Entity() = default;

    // Основные методы
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget& target);
    virtual void shoot(Texture* bulletTexture) = 0;
    virtual void updateBullets();
    
    // Методы для работы с HP
    void takeDamage(int damage);
    bool isAlive() const;
    
    // Методы позиционирования
    void move(float offsetX, float offsetY);
    void set_center(float x, float y);
    
    // Геттеры
    double get_left() { return sprite.get_left(); }
    double get_right() { return sprite.get_right(); }
    double get_top() { return sprite.get_top(); }
    double get_bottom() { return sprite.get_bottom(); }
    double get_width() { return sprite.get_width(); }
    double get_height() { return sprite.get_height(); }
    Vector2f get_center() { return sprite.get_center(); }
    
    const std::vector<Bullet>& getBullets() const { return bullets; }
    std::vector<Bullet>& getBullets() { return bullets; }
    
    // Методы для работы с таймером стрельбы
    int getShootTimer() const { return shootTimer; }
    void setShootTimer(int value) { shootTimer = value; }
    void incrementShootTimer() { shootTimer++; }

    // Геттеры и сеттеры для основных параметров
    int getHP() const { return HP; }
    int getMaxHP() const { return HPMax; }
    void setHP(int hp) { HP = hp; }
    void setMaxHP(int maxHp) { HPMax = maxHp; }
};
