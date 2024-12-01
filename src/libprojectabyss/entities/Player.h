/**
 * @file Player.h
 * @brief Класс игрока с расширенной функциональностью
 */
#pragma once

#include "../core/AnimatedSprite.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Константы для анимации
const sf::IntRect BOUND_IDLE(32 * 4, 0, 32, 48);
const std::pair<int, int> SPRITE_DATA_IDLE(0, 4);
const sf::IntRect BOUND_MOVE_LEFT(32 * 0, 0, 32, 48);
const sf::IntRect BOUND_MOVE_RIGHT(32 * 8, 0, 32, 48);

class Player : public AnimatedSprite {
public:
    // Перечисление состояний игрока
    enum class State {
        IDLE,
        MOVE_LEFT,
        MOVE_RIGHT,
        SHOOTING
    };

private:
    // Физические параметры
    float speed = 300.0f;
    float health = 100.0f;
    int lives = 3;

    // Состояние игрока
    State currentState = State::IDLE;
    bool isInvulnerable = false;
    float invulnerabilityTime = 0.0f;

    // Стрельба
    float shootCooldown = 0.0f;
    const float SHOOT_DELAY = 0.2f;

public:
    // Список пуль игрока
    std::vector<Bullet> bullets;

    /**
     * @brief Конструктор игрока
     * @param texture Текстура игрока
     * @param startPosition Начальная позиция
     */
    Player(const sf::Texture& texture, sf::Vector2f startPosition);

    /**
     * @brief Обновление состояния игрока
     * @param deltaTime Время между кадрами
     */
    void update(float deltaTime);

    /**
     * @brief Обработка ввода
     * @param event Событие SFML
     */
    void handleInput(const sf::Event& event);

    /**
     * @brief Получение урона
     * @param damage Количество урона
     */
    void takeDamage(float damage);

    /**
     * @brief Проверка состояния здоровья
     * @return true Игрок мертв
     */
    bool isDead() const;

    /**
     * @brief Сброс состояния игрока
     */
    void reset();

    /**
     * @brief Стрельба
     */
    void shoot();

    /**
     * @brief Получение текущей позиции
     * @return sf::Vector2f Позиция игрока
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Получение глобальных границ
     * @return sf::FloatRect Границы игрока
     */
    sf::FloatRect getGlobalBounds() const;

    /**
     * @brief Получение урона игрока
     * @return float Урон игрока
     */
    float getDamage() const;

private:
    /**
     * @brief Обновление анимации
     */
    void updateAnimation();

    /**
     * @brief Обновление движения
     * @param deltaTime Время между кадрами
     */
    void updateMovement(float deltaTime);

    /**
     * @brief Обновление стрельбы
     * @param deltaTime Время между кадрами
     */
    void updateShooting(float deltaTime);

    /**
     * @brief Обновление состояния уязвимости
     * @param deltaTime Время между кадрами
     */
    void updateInvulnerability(float deltaTime);
};