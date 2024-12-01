/**
 * @file Entity.h
 * @brief Базовый абстрактный класс игровой сущности
 */
#pragma once
#include "../core/AnimatedSprite.h"
#include "Bullet.h"
#include "BulletManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Entity : public AnimatedSprite {
public:
    // Состояния сущности
    enum class State {
        SPAWNING,   // Появление
        IDLE,       // Простаивание
        MOVING,     // Движение
        ATTACKING,  // Атака
        DEFENDING,  // Защита
        DAMAGED,    // Получение урона
        DYING,      // Умирание
        DEAD        // Мертв
    };

protected:
    // Основные параметры
    State currentState = State::SPAWNING;
    std::unique_ptr<BulletManager> bulletManager;
    
    float health = 100.0f;
    float maxHealth = 100.0f;
    float speed = 100.0f;
    int lives = 3;

    // Параметры урона и защиты
    float damageMultiplier = 1.0f;
    float defenseMultiplier = 1.0f;

    // Временные метки и кулдауны
    float invulnerabilityTime = 0.0f;
    float stateTransitionCooldown = 0.0f;

public:
    // Конструктор по умолчанию
    Entity() = default;

    // Виртуальные методы для переопределения
    virtual void update(float deltaTime) = 0;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void updateMovement(float deltaTime) = 0;
    virtual void updateAnimation() = 0;

    // Методы управления состоянием
    virtual void takeDamage(float damage);
    virtual void heal(float amount);
    virtual bool isDead() const;
    virtual void respawn();

    // Геттеры и сеттеры
    State getState() const { return currentState; }
    void setState(State newState);

    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    void setMaxHealth(float max);

    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed);

    int getLives() const { return lives; }
    void setLives(int newLives);

    // Методы боевой системы
    virtual float getDamage() const;
    virtual float calculateDamage(float baseDamage) const;
    virtual float calculateDefense(float incomingDamage) const;

protected:
    // Защищенные методы для наследников
    virtual void updateStateMachine(float deltaTime);
    virtual void handleStateTransition(State fromState, State toState);
};