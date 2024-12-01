/**
 * @file CollisionManager.h
 * @brief Менеджер обработки столкновений в игре
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Bullet.h"
#include "../core/GameState.h"
/**
 * @brief Типы коллизий в игре
 */
enum class CollisionType {
    PLAYER_ENEMY,      ///< Столкновение игрока с противником
    PLAYER_BULLET,     ///< Попадание пули противника в игрока
    PLAYER_POWERUP,    ///< Подбор бонуса игроком
    ENEMY_BULLET,      ///< Попадание пули игрока в противника
    SCREEN_BOUNDARY    ///< Выход за границы экрана
};

/**
 * @brief Структура для описания столкновения
 */
struct CollisionEvent {
    CollisionType type;    ///< Тип столкновения
    sf::Vector2f position; ///< Позиция столкновения
    float damage;          ///< Урон (если применимо)
};

/**
 * @brief Менеджер коллизий и обработки столкновений
 * 
 * Отвечает за:
 * - Проверку столкновений между объектами
 * - Генерацию событий столкновений
 * - Применение последствий столкновений
 */
class CollisionManager {
private:
    /// Ссылка на игрока
    Player& player;

    /// Список врагов
    std::vector<std::unique_ptr<Enemy>>& enemies;

    /// Ссылка на состояние игры
    GameState& gameState;

    /// Список событий столкновений
    std::vector<CollisionEvent> collisionEvents;

public:
    /**
     * @brief Конструктор менеджера коллизий
     * 
     * @param gamePlayer Ссылка на игрока
     * @param enemyList Список врагов
     * @param currentGameState Состояние игры
     */
    CollisionManager(
        Player& gamePlayer, 
        std::vector<std::unique_ptr<Enemy>>& enemyList,
        GameState& currentGameState
    );

    /**
     * @brief Обновление и проверка коллизий
     * 
     * @param deltaTime Время между кадрами
     */
    void update(float deltaTime);

    /**
     * @brief Получение списка событий столкновений
     * 
     * @return const std::vector<CollisionEvent>& Список событий
     */
    const std::vector<CollisionEvent>& getCollisionEvents() const;

private:
    /**
     * @brief Проверка столкновений пуль игрока с противниками
     */
    void checkPlayerBulletsEnemyCollisions();

    /**
     * @brief Проверка столкновений пуль противников с игроком
     */
    void checkEnemyBulletsPlayerCollisions();

    /**
     * @brief Проверка прямого столкновения игрока с противниками
     */
    void checkPlayerEnemyCollisions();

    /**
     * @brief Проверка выхода объектов за границы экрана
     */
    void checkScreenBoundaryCollisions();

    /**
     * @brief Обработка события столкновения
     * 
     * @param event Событие столкновения
     */
    void processCollisionEvent(const CollisionEvent& event);
};