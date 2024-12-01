/**
 * @file Enemy.h
 * @brief Класс врага с расширенной функциональностью
 */
#pragma once

#include <cmath>
#include "../core/AnimatedSprite.h"
#include "Bullet.h"
#include "Player.h"

// Константы для анимации врага
const sf::IntRect ENEMY_BOUND_IDLE(0, 0, 32, 48);
const sf::IntRect ENEMY_BOUND_ATTACK(32, 0, 32, 48);

class Enemy : public AnimatedSprite {
public:
    // Типы врагов
    enum class Type {
        BASIC,
        SHOOTER,
        FAST,
        BOSS
    };

    // Состояния врага
    enum class State {
        ENTERING,
        IDLE,
        ATTACKING,
        MOVING,
        DYING
    };

private:
    // Физические параметры
    float health = 50.0f;
    float speed = 100.0f;
    float damage = 10.0f;

    // Характеристики врага
    Type enemyType;
    State currentState;

    // Стрельба
    float shootCooldown = 0.0f;
    const float SHOOT_DELAY = 1.0f;

public:
    // Список пуль врага
    std::vector<Bullet> bullets;

    /**
     * @brief Конструктор врага
     * @param texture Текстура врага
     * @param type Тип врага
     * @param startPosition Начальная позиция
     */
    Enemy(const sf::Texture& texture, Type type, sf::Vector2f startPosition);

    /**
     * @brief Обновление состояния врага
     * @param deltaTime Время между кадрами
     * @param player Ссылка на игрока
     */
    void update(float deltaTime, const Player& player);

    /**
     * @brief Получение урона
     * @param damage Количество урона
     */
    void takeDamage(float damage);

    /**
     * @brief Проверка состояния здоровья
     * @return true Враг уничтожен
     */
    bool isDead() const;

    /**
     * @brief Получение позиции
     * @return sf::Vector2f Позиция врага
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Получение глобальных границ
     * @return sf::FloatRect Границы врага
     */
    sf::FloatRect getGlobalBounds() const;

    /**
     * @brief Получение урона врага
     * @return float Урон врага
     */
    float getDamage() const;

private:
    /**
     * @brief Обновление движения
     * @param deltaTime Время между кадрами
     * @param player Ссылка на игрока
     */
    void updateMovement(float deltaTime, const Player& player);

    /**
     * @brief Обновление стрельбы
     * @param deltaTime Время между кадрами
     * @param player Ссылка на игрока
     */
    void updateShooting(float deltaTime, const Player& player);

    /**
     * @brief Обновление анимации
     */
    void updateAnimation();

    /**
     * @brief Выбор траектории движения
     * @param player Ссылка на игрока
     */
    void chooseMovementPattern(const Player& player);
};