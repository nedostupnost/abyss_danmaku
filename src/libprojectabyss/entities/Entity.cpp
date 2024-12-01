/**
 * @file Entity.cpp
 * @brief Реализация базового класса игровой сущности
 */
#include "Entity.h"

void Entity::takeDamage(float damage) {
    if (currentState != State::DYING && currentState != State::DEAD) {
        float actualDamage = calculateDefense(damage);
        health -= actualDamage;

        if (health <= 0) {
            currentState = State::DYING;
            lives--;
        } else {
            currentState = State::DAMAGED;
        }
    }
}

void Entity::heal(float amount) {
    health = std::min(health + amount, maxHealth);
}

bool Entity::isDead() const {
    return currentState == State::DEAD || lives <= 0;
}

void Entity::respawn() {
    if (lives > 0) {
        health = maxHealth;
        currentState = State::SPAWNING;
    }
}

void Entity::setState(State newState) {
    handleStateTransition(currentState, newState);
    currentState = newState;
}

void Entity::setMaxHealth(float max) {
    maxHealth = max;
    health = std::min(health, maxHealth);
}

void Entity::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Entity::setLives(int newLives) {
    lives = newLives;
}

float Entity::getDamage() const {
    return 10.0f * damageMultiplier;
}

float Entity::calculateDamage(float baseDamage) const {
    return baseDamage * damageMultiplier;
}

float Entity::calculateDefense(float incomingDamage) const {
    return incomingDamage * (1.0f / defenseMultiplier);
}

void Entity::updateStateMachine(float deltaTime) {
    stateTransitionCooldown -= deltaTime;

    // Базовая логика обновления состояний
    switch (currentState) {
        case State::SPAWNING:
            if (stateTransitionCooldown <= 0) {
                currentState = State::IDLE;
            }
            break;
        case State::DAMAGED:
            if (stateTransitionCooldown <= 0) {
                currentState = State::IDLE;
            }
            break;
        case State::DYING:
            if (lives <= 0) {
                currentState = State::DEAD;
            }
            break;
        default:
            break;
    }
}

void Entity::handleStateTransition(State fromState, State toState) {
    // Логика переходов между состояниями
    if (fromState != toState) {
        stateTransitionCooldown = 0.5f;  // Время восстановления
    }
}