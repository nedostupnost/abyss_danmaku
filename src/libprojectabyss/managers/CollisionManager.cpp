/**
 * @file CollisionManager.cpp
 * @brief Реализация менеджера коллизий
 */
#include "CollisionManager.h"

CollisionManager::CollisionManager(
    Player& gamePlayer, 
    std::vector<std::unique_ptr<Enemy>>& enemyList,
    GameState& currentGameState
) : 
    player(gamePlayer),
    enemies(enemyList),
    gameState(currentGameState)
{}

void CollisionManager::update(float deltaTime) {
    // Очистка предыдущих событий столкновений
    collisionEvents.clear();

    // Последовательная проверка различных типов коллизий
    checkPlayerBulletsEnemyCollisions();
    checkEnemyBulletsPlayerCollisions();
    checkPlayerEnemyCollisions();
    checkScreenBoundaryCollisions();

    // Обработка всех сгенерированных событий столкновений
    for (const auto& event : collisionEvents) {
        processCollisionEvent(event);
    }
}

const std::vector<CollisionEvent>& CollisionManager::getCollisionEvents() const {
    return collisionEvents;
}

void CollisionManager::checkPlayerBulletsEnemyCollisions() {
    // Проверка каждой пули игрока со всеми противниками
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            sf::FloatRect bulletBounds = bullet.shape.getGlobalBounds();
            sf::FloatRect enemyBounds = enemy->getGlobalBounds();

            if (bulletBounds.intersects(enemyBounds)) {
                collisionEvents.push_back({
                    CollisionType::ENEMY_BULLET,
                    bullet.shape.getPosition(),
                    player.getDamage()
                });
            }
        }
    }
}

void CollisionManager::checkEnemyBulletsPlayerCollisions() {
    // Проверка пуль каждого противника с игроком
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            sf::FloatRect bulletBounds = bullet.shape.getGlobalBounds();
            sf::FloatRect playerBounds = player.getGlobalBounds();

            if (bulletBounds.intersects(playerBounds)) {
                collisionEvents.push_back({
                    CollisionType::PLAYER_BULLET,
                    bullet.shape.getPosition(),
                    enemy->getDamage()
                });
            }
        }
    }
}

void CollisionManager::checkPlayerEnemyCollisions() {
    // Проверка прямого столкновения игрока с противниками
    for (auto& enemy : enemies) {
        sf::FloatRect enemyBounds = enemy->getGlobalBounds();
        sf::FloatRect playerBounds = player.getGlobalBounds();

        if (enemyBounds.intersects(playerBounds)) {
            collisionEvents.push_back({
                CollisionType::PLAYER_ENEMY,
                enemy->getPosition(),
                enemy->getDamage()
            });
        }
    }
}

void CollisionManager::checkScreenBoundaryCollisions() {
    // Проверка выхода за границы экрана для пуль игрока
    for (auto& bullet : player.bullets) {
        if (bullet.shape.getPosition().y < 0) {
            collisionEvents.push_back({
                CollisionType::SCREEN_BOUNDARY,
                bullet.shape.getPosition(),
                0.0f
            });
        }
    }

    // Проверка выхода за границы экрана для пуль противников
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            if (bullet.shape.getPosition().y > gameState.windowHeight) {
                collisionEvents.push_back({
                    CollisionType::SCREEN_BOUNDARY,
                    bullet.shape.getPosition(),
                    0.0f
                });
            }
        }
    }
}

void CollisionManager::processCollisionEvent(const CollisionEvent& event) {
    switch (event.type) {
        case CollisionType::ENEMY_BULLET:
            // Нанесение урона противнику и удаление пули
            for (auto& enemy : enemies) {
                if (enemy->getGlobalBounds().contains(event.position)) {
                    enemy->takeDamage(event.damage);
                    break;
                }
            }
            break;

        case CollisionType::PLAYER_BULLET:
            // Нанесение урона игроку
            player.takeDamage(event.damage);
            break;

        case CollisionType::PLAYER_ENEMY:
            // Урон игроку при столкновении с противником
            player.takeDamage(event.damage);
            break;

        case CollisionType::SCREEN_BOUNDARY:
            // Удаление объектов, вышедших за границы экрана
            player.bullets.erase(
                std::remove_if(player.bullets.begin(), player.bullets.end(),
                    [&event](const Bullet& bullet) {
                        return bullet.shape.getPosition() == event.position;
                    }),
                player.bullets.end()
            );
            break;

        default:
            break;
    }
}