#include "Enemy.h"
#include "Logger.h"

Enemy::Enemy(sf::Texture* texture, sf::Vector2f pos, EnemyMovementPattern movePattern, EnemyShootPattern shootPattern) {
    this->HPMax = 1;
    this->HP = this->HPMax;
    this->damageTimer = 0;
    this->shootTimer = 0;
    this->movePattern = movePattern;
    this->shootPattern = shootPattern;
    this->rotationAngle = 0.f;
    this->movementTimer = 0.f;
    this->speed = 3.f;
    
    if (!texture) {
        Logger::log(LogLevel::ERROR, "Enemy: Не удалось инициализировать текстуру врага");
        return;
    }
    
    // Инициализация спрайта
    sprite.init_texture(texture);
    sprite.init(sf::IntRect(0, 0, 32, 32), 0, 4, 8);
    sprite.set_center(pos.x, pos.y);
    
    // Сохраняем начальную позицию для некоторых паттернов движения
    baseX = pos.x;
    baseY = pos.y;
    
    Logger::log(LogLevel::DEBUG, "Enemy: Создан враг с паттерном движения " + 
                std::to_string(movePattern) + " и паттерном стрельбы " + 
                std::to_string(shootPattern));
}

void Enemy::updateMovement() {
    float x = sprite.get_center().x;
    float y = sprite.get_center().y;
    
    switch (movePattern) {
        case PATTERN_STRAIGHT:
            y += speed;
            break;
            
        case PATTERN_SINE: {
            y += speed;
            x = baseX + sin(movementTimer * 0.05f) * 100.f;
            movementTimer += 1.f;
            break;
        }
            
        case PATTERN_ZIGZAG: {
            y += speed;
            x = baseX + (fmod(movementTimer, 120.f) < 60.f ? 1 : -1) * speed * 2.f;
            movementTimer += 1.f;
            break;
        }
            
        case PATTERN_CIRCLE: {
            float radius = 100.f;
            x = baseX + cos(movementTimer * 0.05f) * radius;
            y = baseY + sin(movementTimer * 0.05f) * radius;
            movementTimer += 1.f;
            break;
        }
            
        case PATTERN_WAVE: {
            y += speed * 0.5f;
            x = baseX + sin(y * 0.05f) * 100.f;
            break;
        }
            
        case PATTERN_SPIRAL: {
            float radius = movementTimer * 0.5f;
            x = baseX + cos(movementTimer * 0.05f) * radius;
            y = baseY + sin(movementTimer * 0.05f) * radius;
            movementTimer += 1.f;
            break;
        }
    }
    
    sprite.set_center(x, y);
}

void Enemy::shoot(sf::Texture* bulletTexture, const sf::Vector2f& playerPos) {
    if (!bulletTexture) {
        Logger::log(LogLevel::ERROR, "Enemy: Не удалось создать пулю - отсутствует текстура");
        return;
    }

    sf::Vector2f pos = sprite.get_center();
    
    switch (shootPattern) {
        case SHOOT_SINGLE: {
            bullets.push_back(Bullet(bulletTexture, pos, sf::Vector2f(0.f, 5.f)));
            break;
        }
            
        case SHOOT_SPREAD: {
            const int numBullets = 5;
            const float spreadAngle = 60.f;
            float angleStep = spreadAngle / (numBullets - 1);
            float startAngle = -spreadAngle / 2;
            
            for (int i = 0; i < numBullets; ++i) {
                float angle = startAngle + i * angleStep;
                float radians = angle * 3.14159f / 180.f;
                sf::Vector2f velocity(sin(radians) * 5.f, cos(radians) * 5.f);
                bullets.push_back(Bullet(bulletTexture, pos, velocity));
            }
            break;
        }
            
        case SHOOT_CIRCLE: {
            const int numBullets = 8;
            float angleStep = 360.f / numBullets;
            
            for (int i = 0; i < numBullets; ++i) {
                float angle = i * angleStep;
                float radians = angle * 3.14159f / 180.f;
                sf::Vector2f velocity(sin(radians) * 5.f, cos(radians) * 5.f);
                bullets.push_back(Bullet(bulletTexture, pos, velocity));
            }
            break;
        }
            
        case SHOOT_SPIRAL: {
            float radians = rotationAngle * 3.14159f / 180.f;
            sf::Vector2f velocity(sin(radians) * 5.f, cos(radians) * 5.f);
            bullets.push_back(Bullet(bulletTexture, pos, velocity));
            rotationAngle += 30.f;
            break;
        }
            
        case SHOOT_AIMED: {
            sf::Vector2f direction = playerPos - pos;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction /= length;
                direction *= 5.f;
                bullets.push_back(Bullet(bulletTexture, pos, direction));
            }
            break;
        }
    }
    
    Logger::log(LogLevel::DEBUG, "Enemy: Выстрел с паттерном " + std::to_string(shootPattern));
}

void Enemy::update() {
    updateMovement();
    
    // Обновление таймера урона
    if (damageTimer > 0)
        --damageTimer;
        
    // Обновление таймера стрельбы
    if (shootTimer > 0)
        --shootTimer;
        
    // Обновление пуль
    for (size_t i = 0; i < bullets.size(); ++i) {
        bullets[i].update();
        if (bullets[i].isOffscreen(sf::Vector2u(800, 600))) { // TODO: передавать размер окна
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
}

void Enemy::draw(sf::RenderTarget& target) const {
    // Отрисовка спрайта врага
    sprite.draw(target);
    
    // Отрисовка пуль
    for (const auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}