#include "Enemy.h"

Enemy::Enemy(Texture* texture, Vector2f pos, EnemyMovementPattern pattern) {
    this->HPMax = 1;
    this->HP = this->HPMax;
    this->damageTimer = 10;
    this->shootTimer = 0;
    
    // Инициализация анимированного спрайта
    sprite.init_texture(texture);
    sprite.init(ENEMY_BOUND_IDLE, 
               ENEMY_SPRITE_DATA_IDLE.first, 
               ENEMY_SPRITE_DATA_IDLE.second, 
               SPRITE_FRAME_RATE);
    
    sprite.set_center(pos.x, pos.y);
    startPos = pos;
    
    // Инициализация параметров движения
    this->pattern = pattern;
    speed = 3.0f;
    time = 0.0f;
    direction = Vector2f(0.f, 1.f);
}

void Enemy::updateMovement() {
    time += 0.016f; // Примерно 60 FPS
    float x = sprite.get_center().x;
    float y = sprite.get_center().y;

    switch (pattern) {
        case PATTERN_STRAIGHT:
            y += speed;
            break;
            
        case PATTERN_SINE:
            x = startPos.x + sin(time * 2.0f) * 100.0f;
            y += speed * 0.5f;
            break;
            
        case PATTERN_ZIGZAG:
            if (fmod(time, 2.0f) < 1.0f)
                x += speed;
            else
                x -= speed;
            y += speed * 0.5f;
            break;
            
        case PATTERN_CIRCLE:
            x = startPos.x + cos(time * 2.0f) * 50.0f;
            y = startPos.y + sin(time * 2.0f) * 50.0f + time * speed;
            break;
    }

    sprite.set_center(x, y);
}

void Enemy::shoot(Texture* bulletTexture) {
    if (shootTimer >= 60) { // Стреляем каждую секунду
        Vector2f pos = sprite.get_center();
        bullets.push_back(Bullet(bulletTexture, pos));
        shootTimer = 0;
    }
}

void Enemy::update() {
    // Обновление анимации
    sprite.tick();
    
    // Обновление таймера стрельбы
    shootTimer++;
    
    // Обновление движения
    updateMovement();
    
    // Обновление пуль
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i].shape.move(0.f, 5.f); // Пули летят вниз
        
        // Удаление пуль за пределами экрана
        if (bullets[i].shape.getPosition().y > 600) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Enemy::draw(sf::RenderTarget& target) {
    sprite.draw(target);
    for (auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}