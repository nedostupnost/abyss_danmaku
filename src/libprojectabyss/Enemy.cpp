#include "Enemy.h"

Enemy::Enemy(Texture* texture, Vector2f pos, 
            EnemyMovementPattern movePattern, BulletPattern shootPattern,
            float enemySpeed, int enemyHP)
    : Entity(texture, pos) {
    this->HP = enemyHP;
    this->damageTimer = 10;
    this->shootInterval = 40;
    
    // Инициализация параметров стрельбы
    this->bulletPattern = shootPattern;
    this->bulletSpeed = 5.f;
    
    // Инициализация анимированного спрайта
    sprite.init(ENEMY_BOUND_IDLE, 
               ENEMY_SPRITE_DATA_IDLE.first, 
               ENEMY_SPRITE_DATA_IDLE.second, 
               SPRITE_FRAME_RATE);
    
    startPos = pos;
    
    // Инициализация параметров движения
    this->movementPattern = movePattern;
    this->speed = enemySpeed;
    this->time = 0.0f;
    this->direction = Vector2f(0.f, 1.f);
    this->amplitude = 100.0f;  // Амплитуда волновых движений
    this->frequency = 2.0f;    // Частота волновых движений
}

void Enemy::updateMovement() {
    time += 0.016f; // Примерно 60 FPS
    float x = sprite.get_center().x;
    float y = sprite.get_center().y;

    // Применяем выбранный паттерн движения
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

    // Проверяем границы экрана
    float minX = get_width() / 2.0f;
    float maxX = 800.0f - get_width() / 2.0f;
    x = std::max(minX, std::min(x, maxX));
    
    sprite.set_center(x, y);
}

void Enemy::shoot(Texture* bulletTexture, const Vector2f& playerPos) {
    Vector2f pos = sprite.get_center();

    switch (bulletPattern) {
        case BULLET_SINGLE: {
            // Одиночный выстрел направлен в игрока
            Bullet b(bulletTexture, pos, BULLET_SINGLE_TYPE, false);
            Vector2f targetDir = playerPos - pos;
            float length = std::sqrt(targetDir.x * targetDir.x + targetDir.y * targetDir.y);
            b.direction = Vector2f(targetDir.x / length, targetDir.y / length);
            bullets.push_back(b);
            break;
        }

        case BULLET_CIRCLE: {
            // Круговой паттерн с 16 пулями для более плотного круга
            const int numBullets = 16;
            for (int i = 0; i < numBullets; ++i) {
                Bullet b(bulletTexture, pos, BULLET_CIRCLE_TYPE, false);
                float angle = (i * 360.f / numBullets) * 3.14159f / 180.f;
                b.direction = Vector2f(std::cos(angle), std::sin(angle));
                bullets.push_back(b);
            }
            break;
        }

        case BULLET_SPIRAL: {
            // Спиральный паттерн с 3 пулями для создания спирали
            const int arms = 3;
            for (int i = 0; i < arms; ++i) {
                Bullet b(bulletTexture, pos, BULLET_SPIRAL_TYPE, false);
                float baseAngle = shootTimer * 180.f * 3.14159f / 180.f;
                float angle = baseAngle + (i * 360.f / arms) * 3.14159f / 180.f;
                b.direction = Vector2f(std::cos(angle), std::sin(angle));
                bullets.push_back(b);
            }
            break;
        }

        case BULLET_FAN: {
            // Веерный паттерн с 7 пулями в широком секторе
            const int numBullets = 7;
            const float spreadAngle = 90.f; // Общий угол веера в градусах
            for (int i = 0; i < numBullets; ++i) {
                Bullet b(bulletTexture, pos, BULLET_FAN_TYPE, false);
                float angle = (-spreadAngle/2 + i * spreadAngle/(numBullets-1)) * 3.14159f / 180.f;
                Vector2f baseDir = playerPos - pos;
                float baseAngle = std::atan2(baseDir.y, baseDir.x);
                float finalAngle = baseAngle + angle;
                b.direction = Vector2f(std::cos(finalAngle), std::sin(finalAngle));
                bullets.push_back(b);
            }
            break;
        }

        case BULLET_WAVE:
            {
                Bullet b(bulletTexture, pos, BULLET_WAVE_TYPE, false);
                b.wavePattern = true;
                bullets.push_back(b);
            }
            break;

        case BULLET_AIMED: {
            // Прицельный выстрел в игрока с небольшим разбросом
            const int numBullets = 3;
            Vector2f targetDir = playerPos - pos;
            float baseAngle = std::atan2(targetDir.y, targetDir.x);
            
            for (int i = 0; i < numBullets; ++i) {
                Bullet b(bulletTexture, pos, BULLET_AIMED_TYPE, false);
                float angleOffset = (i - numBullets/2) * 10.f * 3.14159f / 180.f;
                float finalAngle = baseAngle + angleOffset;
                b.direction = Vector2f(std::cos(finalAngle), std::sin(finalAngle));
                bullets.push_back(b);
            }
            break;
        }
    }

    shootTimer++;
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

    updateBullets();
}

void Enemy::update() {
    // Базовая реализация - просто обновляем спрайт и пули
    sprite.tick();
    updateMovement();
    updateBullets();
}

void Enemy::shoot(Texture* bulletTexture) {
    // Базовая реализация - стреляем прямо вниз
    Vector2f pos = sprite.get_center();
    Bullet b(bulletTexture, pos, BULLET_SINGLE_TYPE, false);
    b.direction = Vector2f(0.f, 1.f);
    bullets.push_back(b);
}

void Enemy::draw(sf::RenderTarget& target) {
    sprite.draw(target);
    for (auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}