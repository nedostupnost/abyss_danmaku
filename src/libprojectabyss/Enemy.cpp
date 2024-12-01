#include "Enemy.h"

Enemy::Enemy(Texture* texture, Vector2f pos, EnemyMovementPattern pattern) {
    this->HPMax = 1;
    this->HP = this->HPMax;
    this->damageTimer = 10;
    this->shootTimer = 0;
    
    // Инициализация параметров стрельбы
    this->bulletPattern = BULLET_SINGLE;
    this->bulletAngle = 0.f;
    this->bulletSpeed = 5.f;
    this->bulletsPerShot = 8;
    this->type = ENEMY_BASIC;  // По умолчанию базовый тип
    
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
        
        switch (bulletPattern) {
            case BULLET_SINGLE: {
                // Простой выстрел вниз
                bullets.push_back(Bullet(bulletTexture, pos, BULLET_TYPE_1));
                bullets.back().shape.setScale(0.5f, 0.5f);
                break;
            }
            
            case BULLET_CIRCLE: {
                // Круговой паттерн
                float angleStep = 360.f / bulletsPerShot;
                for (int i = 0; i < bulletsPerShot; ++i) {
                    float angle = i * angleStep;
                    bullets.push_back(Bullet(bulletTexture, pos, BULLET_TYPE_2));
                    bullets.back().shape.setScale(0.5f, 0.5f);
                    float radians = angle * 3.14159f / 180.f;
                    bullets.back().direction = Vector2f(cos(radians), sin(radians));
                }
                break;
            }
            
            case BULLET_SPIRAL: {
                // Спиральный паттерн
                float angle = bulletAngle;
                for (int i = 0; i < 3; ++i) {
                    bullets.push_back(Bullet(bulletTexture, pos, BULLET_TYPE_3));
                    bullets.back().shape.setScale(0.5f, 0.5f);
                    float radians = angle * 3.14159f / 180.f;
                    bullets.back().direction = Vector2f(cos(radians), sin(radians));
                    angle += 30.f;
                }
                bulletAngle += 20.f;
                break;
            }
            
            case BULLET_FAN: {
                // Веерный паттерн
                float spreadAngle = 60.f;
                float angleStep = spreadAngle / (bulletsPerShot - 1);
                float startAngle = 90.f - spreadAngle / 2;
                
                for (int i = 0; i < bulletsPerShot; ++i) {
                    float angle = startAngle + i * angleStep;
                    bullets.push_back(Bullet(bulletTexture, pos, BULLET_TYPE_4));
                    bullets.back().shape.setScale(0.5f, 0.5f);
                    float radians = angle * 3.14159f / 180.f;
                    bullets.back().direction = Vector2f(cos(radians), sin(radians));
                }
                break;
            }
            
            case BULLET_WAVE: {
                // Волновой паттерн
                bullets.push_back(Bullet(bulletTexture, pos, BULLET_TYPE_5));
                bullets.back().shape.setScale(0.5f, 0.5f);
                bullets.back().wavePattern = true;
                bullets.back().waveTime = 0.f;
                bullets.back().direction = Vector2f(0.f, 1.f);
                break;
            }
        }
        
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
        bullets[i].update();  // Используем новый метод update
        
        // Удаление пуль за пределами экрана
        Vector2f pos = bullets[i].shape.getPosition();
        if (pos.y > 600 || pos.y < 0 || pos.x < 0 || pos.x > 800) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Enemy::setType(EnemyType newType) {
    type = newType;
    
    // Настраиваем параметры в зависимости от типа врага
    switch(type) {
        case ENEMY_BASIC:
            HPMax = 1;
            bulletPattern = BULLET_SINGLE;
            bulletSpeed = 5.f;
            speed = 3.0f;
            break;
            
        case ENEMY_SPINNER:
            HPMax = 3;
            bulletPattern = BULLET_CIRCLE;
            bulletsPerShot = 8;
            bulletSpeed = 4.f;
            speed = 2.0f;
            break;
            
        case ENEMY_SPIRAL:
            HPMax = 2;
            bulletPattern = BULLET_SPIRAL;
            bulletSpeed = 4.5f;
            speed = 2.5f;
            break;
            
        case ENEMY_SNIPER:
            HPMax = 2;
            bulletPattern = BULLET_FAN;
            bulletsPerShot = 5;
            bulletSpeed = 6.f;
            speed = 3.5f;
            break;
            
        case ENEMY_WAVE:
            HPMax = 2;
            bulletPattern = BULLET_WAVE;
            bulletSpeed = 4.f;
            speed = 2.0f;
            break;
    }
    
    HP = HPMax;  // Восстанавливаем здоровье при смене типа
}

void Enemy::setBulletPattern(BulletPattern pattern) {
    bulletPattern = pattern;
}

void Enemy::draw(sf::RenderTarget& target) {
    sprite.draw(target);
    for (auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}