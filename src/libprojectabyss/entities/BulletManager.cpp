/**
 * @file BulletManager.cpp
 * @brief Реализация менеджера пуль
 */
#include "BulletManager.h"
#include <algorithm>

Bullet* BulletManager::createBullet(
    Bullet::Type bulletType, 
    sf::Vector2f position, 
    sf::Vector2f direction, 
    float speed
) {
    // Проверка соответствия типа пули типу менеджера
    if (managerType != Type::MIXED && 
        ((managerType == Type::PLAYER && bulletType != Bullet::Type::PLAYER) ||
         (managerType == Type::ENEMY && bulletType != Bullet::Type::ENEMY))) {
        return nullptr;
    }

    Bullet* bullet = nullptr;

    // Поиск неактивной пули в пуле
    auto it = std::find_if(bulletPool.begin(), bulletPool.end(), 
        [](const auto& b) { return b->getState() == Bullet::State::INACTIVE; });

    if (it != bulletPool.end()) {
        bullet = it->get();
        bullet->setState(Bullet::State::ACTIVE);
        bullet->setPosition(position);
        bullet->setDirection(direction);
        bullet->setSpeed(speed);
        bullet->setType(bulletType);
        bullet->setScreenBounds(screenBounds);
    } 
    else if (bulletPool.size() < MAX_POOL_SIZE) {
        bulletPool.push_back(std::make_unique<Bullet>(bulletType, position, direction, speed));
        bullet = bulletPool.back().get();
        bullet->setScreenBounds(screenBounds);
    }

    if (bullet) {
        activeBullets.push_back(bullet);
    }

    return bullet;
}

void BulletManager::update(float deltaTime) {
    for (auto it = activeBullets.begin(); it != activeBullets.end();) {
        (*it)->update(deltaTime);

        if ((*it)->isOutOfBounds()) {
            (*it)->setState(Bullet::State::INACTIVE);
            it = activeBullets.erase(it);
        } else {
            ++it;
        }
    }
}

const std::vector<Bullet*>& BulletManager::getActiveBullets() const {
    return activeBullets;
}

void BulletManager::clear() {
    for (auto& bullet : activeBullets) {
        bullet->setState(Bullet::State::INACTIVE);
    }
    activeBullets.clear();
}

size_t BulletManager::getBulletCount() const {
    return activeBullets.size();
}

void BulletManager::setScreenBounds(sf::Vector2u bounds) {
    screenBounds = bounds;
    for (auto& bullet : bulletPool) {
        bullet->setScreenBounds(bounds);
    }
}