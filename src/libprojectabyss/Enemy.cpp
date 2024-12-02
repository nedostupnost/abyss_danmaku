#include "Enemy.h"

Enemy::Enemy(Texture* texture, Vector2f pos, 
            EnemyMovementPattern movePattern, BulletPattern shootPattern) {
    this->HP = 1;
    this->damageTimer = 10;
    this->shootTimer = 0;
    this->shootInterval = 20; // Добавляем переменную shootInterval
    
    // Инициализация параметров стрельбы
    this->bulletPattern = shootPattern;
    this->bulletSpeed = 5.f;
    
    // Инициализация анимированного спрайта
    sprite.init_texture(texture);
    sprite.init(ENEMY_BOUND_IDLE, 
               ENEMY_SPRITE_DATA_IDLE.first, 
               ENEMY_SPRITE_DATA_IDLE.second, 
               SPRITE_FRAME_RATE);
    
    sprite.set_center(pos.x, pos.y);
    startPos = pos;
    
    // Инициализация параметров движения
    this->movementPattern = movePattern;
    this->speed = 3.0f;
    this->time = 0.0f;
    this->direction = Vector2f(0.f, 1.f);
    this->amplitude = 100.0f;  // Амплитуда волновых движений
    this->frequency = 2.0f;    // Частота волновых движений
}

void Enemy::updateMovement() {
    time += 0.016f; // Примерно 60 FPS
    float x = sprite.get_center().x;
    float y = sprite.get_center().y;

    switch (movementPattern) {
        case PATTERN_STRAIGHT:
            y += speed;
            break;
            
        case PATTERN_SINE:
            x = startPos.x + amplitude * sin(time * frequency);
            y += speed * 0.5f;
            break;
            
        case PATTERN_ZIGZAG: {
            float period = 2.0f;
            float t = fmod(time, period);
            if (t < period/2)
                x += speed;
            else
                x -= speed;
            y += speed * 0.5f;
            break;
        }
            
        case PATTERN_CIRCLE: {
            float radius = 50.0f;
            x = startPos.x + radius * cos(time * frequency);
            y = startPos.y + radius * sin(time * frequency) + time * speed * 0.5f;
            break;
        }

        case PATTERN_EIGHT: {
            float t = time * frequency;
            x = startPos.x + amplitude * sin(t);
            y = startPos.y + (amplitude/2) * sin(2 * t) + time * speed * 0.3f;
            break;
        }
    }

    sprite.set_center(x, y);
}

void Enemy::shoot(Texture* bulletTexture, const Vector2f& playerPos) {
    Vector2f pos = sprite.get_center();
    pos.x += sprite.get_width() / 2.f;
    pos.y += sprite.get_height() / 2.f;

    switch (bulletPattern) {
        case BULLET_SINGLE:
            bullets.push_back(Bullet(bulletTexture, pos, BULLET_SINGLE_TYPE, false));
            break;

        case BULLET_CIRCLE:
            for (int i = 0; i < 8; ++i) {
                Bullet b(bulletTexture, pos, BULLET_CIRCLE_TYPE, false);
                float angle = i * 45.f * 3.14159f / 180.f;
                b.direction = Vector2f(std::cos(angle), std::sin(angle));
                bullets.push_back(b);
            }
            break;

        case BULLET_SPIRAL:
            {
                Bullet b(bulletTexture, pos, BULLET_SPIRAL_TYPE, false);
                float angle = shootTimer * 10.f * 3.14159f / 180.f;
                b.direction = Vector2f(std::cos(angle), std::sin(angle));
                bullets.push_back(b);
            }
            break;

        case BULLET_FAN:
            for (int i = -2; i <= 2; ++i) {
                Bullet b(bulletTexture, pos, BULLET_FAN_TYPE, false);
                float angle = i * 15.f * 3.14159f / 180.f;
                b.direction = Vector2f(std::sin(angle), 1.f);
                b.direction /= std::sqrt(b.direction.x * b.direction.x + b.direction.y * b.direction.y);
                bullets.push_back(b);
            }
            break;

        case BULLET_WAVE:
            {
                Bullet b(bulletTexture, pos, BULLET_WAVE_TYPE, false);
                b.wavePattern = true;
                bullets.push_back(b);
            }
            break;

        case BULLET_AIMED:
            {
                Bullet b(bulletTexture, pos, BULLET_AIMED_TYPE, false);
                Vector2f direction = playerPos - pos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                b.direction = direction / length;
                bullets.push_back(b);
            }
            break;
    }
}

void Enemy::update(const Vector2f& playerPos, sf::Texture* bulletTexture) {
    sprite.tick();
    shootTimer++;
    updateMovement();

    // Добавляем задержку между выстрелами
    if (shootTimer >= shootInterval) {
        shoot(bulletTexture, playerPos);
        shootTimer = 0;
    }

    // Обновление пуль
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i].update();

        // Удаление пуль за пределами экрана
        Vector2f pos = bullets[i].shape.getPosition();
        if (pos.y > 600 || pos.y < 0 || pos.x < 0 || pos.x > 800) {
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