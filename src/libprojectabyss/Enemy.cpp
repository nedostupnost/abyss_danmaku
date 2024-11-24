#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(Texture* texture, Vector2f pos) {
    this->HPMax = 1;
    this->HP = this->HPMax;
    this->damageTimer = 10;
    
    // Инициализация анимированного спрайта
    sprite.init_texture(texture);
    sprite.init(ENEMY_BOUND_IDLE, 
               ENEMY_SPRITE_DATA_IDLE.first, 
               ENEMY_SPRITE_DATA_IDLE.second, 
               SPRITE_FRAME_RATE);
    
    sprite.set_center(pos.x, pos.y);
    
    // Инициализация параметров движения
    speed = 3.0f;
    direction = Vector2f(1.0f, 0.0f); // Движение вправо по умолчанию
}

void Enemy::update() {
    // Обновление анимации
    sprite.tick();
    
    // Движение врага
    sprite.move(direction.x * speed, direction.y * speed);
    
    // Проверка границ экрана и смена направления
    if (sprite.get_left() < 0) {
        direction.x = 1.0f;
    }
    else if (sprite.get_right() > 800) {
        direction.x = -1.0f;
    }
}

void Enemy::draw(sf::RenderTarget& target) {
    sprite.draw(target);
}