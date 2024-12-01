#include "Player.h"
#include <cmath>
#include <algorithm>

Player::Player(sf::Texture* texture, sf::Vector2u windowSize) {
    this->HPMax = 10;
    this->HP = this->HPMax;
    this->shootTimer = 0;
    
    // Инициализация спрайта
    sprite.init_texture(texture);
    sprite.init(sf::IntRect(0, 0, 32, 48), 0, 4, 8);
    sprite.set_center(windowSize.x / 2.f, windowSize.y - 100.f);
    
    // Инициализация состояний движения
    for(int i = 0; i < 4; i++) {
        move_states[i] = false;
    }
    
    lastMoveDirection = sf::Vector2f(0.f, -1.f);  // По умолчанию смотрим вверх
}

void Player::move() {
    float dx = 0.f, dy = 0.f;
    float speed = 5.f;

    // Определяем направление движения на основе нажатых клавиш
    if (move_states[0]) dy -= speed;
    if (move_states[1]) dx += speed;
    if (move_states[2]) dy += speed;
    if (move_states[3]) dx -= speed;

    // Если есть движение, обновляем lastMoveDirection
    if (dx != 0.f || dy != 0.f) {
        lastMoveDirection = sf::Vector2f(dx, dy);
        float length = std::sqrt(lastMoveDirection.x * lastMoveDirection.x + 
                          lastMoveDirection.y * lastMoveDirection.y);
        lastMoveDirection /= length;
    }

    // Ограничиваем движение в пределах экрана
    sf::Vector2f newPos = sprite.get_center();
    newPos.x += dx;
    newPos.y += dy;
    
    // Проверяем границы экрана
    float halfWidth = sprite.get_width() / 2.f;
    float halfHeight = sprite.get_height() / 2.f;
    newPos.x = std::max(halfWidth, std::min<float>(newPos.x, 800.f - halfWidth));
    newPos.y = std::max(halfHeight, std::min<float>(newPos.y, 600.f - halfHeight));
    
    sprite.set_center(newPos.x, newPos.y);
}

sf::Vector2f Player::getShootDirection() const {
    // Если игрок не двигается, стреляем вверх
    if (lastMoveDirection.x == 0.f && lastMoveDirection.y == 0.f) {
        return sf::Vector2f(0.f, -1.f);
    }
    return lastMoveDirection;
}

void Player::shoot(sf::Texture* bulletTexture) {
    if (shootTimer >= 10) {  // Задержка между выстрелами
        sf::Vector2f pos = sprite.get_center();
        
        // Создаем пулю, летящую вверх
        Bullet b(bulletTexture, pos);
        float bulletSpeed = 10.f;
        b.velocity = sf::Vector2f(0.f, -bulletSpeed);  // Всегда стреляем вверх
        bullets.push_back(b);
        
        shootTimer = 0;
    }
}

void Player::update() {
    sprite.tick();
    shootTimer++;

    // Обновляем пули
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i].update();
        
        // Удаляем пули за пределами экрана
        if (bullets[i].isOffscreen(sf::Vector2u(800, 600))) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Player::set_move_state(int dir, bool state) {
    if (dir >= 0 && dir < 4) {
        move_states[dir] = state;
    }
}

void Player::draw(sf::RenderTarget& target) {
    sprite.draw(target);
    for (auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}