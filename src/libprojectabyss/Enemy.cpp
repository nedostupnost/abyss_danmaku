#include "Enemy.h"

Enemy::Enemy(sf::Texture* texture, sf::Vector2f pos, EnemyMovementPattern movePattern, EnemyShootPattern shootPattern) {
    this->HPMax = 1;
    this->HP = this->HPMax;
    this->damageTimer = 10;
    this->shootTimer = 0;
    this->rotationAngle = 0.0f;
    
    sprite.init_texture(texture);
    sprite.init(ENEMY_BOUND_IDLE, 
               ENEMY_SPRITE_DATA_IDLE.first, 
               ENEMY_SPRITE_DATA_IDLE.second, 
               SPRITE_FRAME_RATE);
    
    sprite.set_center(pos.x, pos.y);
    startPos = pos;
    
    this->movePattern = movePattern;
    this->shootPattern = shootPattern;
    speed = 3.0f;
    time = 0.0f;
    direction = sf::Vector2f(0.f, 1.f);
}

void Enemy::updateMovement() {
    time += 0.016f;
    float x = sprite.get_center().x;
    float y = sprite.get_center().y;

    switch (movePattern) {
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

        case PATTERN_WAVE:
            x = startPos.x + sin(time * 3.0f) * 150.0f;
            y = startPos.y + time * speed;
            break;

        case PATTERN_SPIRAL:
            x = startPos.x + cos(time) * (50.0f + time * 10.0f);
            y = startPos.y + sin(time) * (50.0f + time * 10.0f);
            break;
    }

    sprite.set_center(x, y);
}

void Enemy::shoot(sf::Texture* bulletTexture, const sf::Vector2f& playerPos) {
    if (shootTimer >= 30) {
        sf::Vector2f pos = sprite.get_center();
        
        switch (shootPattern) {
            case SHOOT_SINGLE: {
                bullets.push_back(Bullet(bulletTexture, pos));
                break;
            }
            
            case SHOOT_SPREAD: {
                for (int i = -2; i <= 2; i++) {
                    Bullet b(bulletTexture, pos);
                    float angle = i * 15.0f;
                    b.velocity = sf::Vector2f(sin(angle * 3.14159f / 180.0f) * 5.0f,
                                         cos(angle * 3.14159f / 180.0f) * 5.0f);
                    bullets.push_back(b);
                }
                break;
            }
            
            case SHOOT_CIRCLE: {
                for (int i = 0; i < 8; i++) {
                    Bullet b(bulletTexture, pos);
                    float angle = i * 45.0f;
                    b.velocity = sf::Vector2f(sin(angle * 3.14159f / 180.0f) * 5.0f,
                                         cos(angle * 3.14159f / 180.0f) * 5.0f);
                    bullets.push_back(b);
                }
                break;
            }
            
            case SHOOT_SPIRAL: {
                Bullet b(bulletTexture, pos);
                b.velocity = sf::Vector2f(sin(rotationAngle * 3.14159f / 180.0f) * 5.0f,
                                     cos(rotationAngle * 3.14159f / 180.0f) * 5.0f);
                bullets.push_back(b);
                rotationAngle += 30.0f;
                break;
            }
            
            case SHOOT_AIMED: {
                sf::Vector2f direction = playerPos - pos;
                float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                direction /= length;
                
                Bullet b(bulletTexture, pos);
                b.velocity = direction * 5.0f;
                bullets.push_back(b);
                break;
            }
        }
        
        shootTimer = 0;
    }
}

void Enemy::update() {
    sprite.tick();
    shootTimer++;
    updateMovement();
    
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i].shape.move(bullets[i].velocity);
        
        sf::Vector2f bulletPos = bullets[i].shape.getPosition();
        if (bulletPos.y > 600 || bulletPos.y < 0 || 
            bulletPos.x > 800 || bulletPos.x < 0) {
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