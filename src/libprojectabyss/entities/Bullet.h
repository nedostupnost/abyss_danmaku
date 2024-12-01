/**
 * @file Bullet.h
 * @brief Класс пули с расширенной функциональностью
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Bullet {
public:
    // Типы пуль
    enum class Type {
        PLAYER,     // Пули игрока
        ENEMY,      // Пули врага
        SPECIAL     // Специальные пули
    };

    // Состояния пули
    enum class State {
        ACTIVE,     // Активная пуля
        INACTIVE,   // Неактивная пуля
        EXPLODING   // Пуля при взрыве
    };

private:
    sf::Vector2f position;      // Позиция пули
    sf::Vector2f direction;     // Направление движения
    float speed = 500.0f;       // Скорость пули
    Type type = Type::PLAYER;   // Тип пули
    State currentState = State::INACTIVE;  // Текущее состояние
    float damage = 10.0f;       // Урон пули
    sf::Sprite sprite;          // Спрайт пули
    sf::Vector2u screenBounds = sf::Vector2u(800, 600);  // Границы экрана

public:
    // Конструкторы
    Bullet() = default;
    Bullet(Type bulletType, sf::Vector2f startPos, sf::Vector2f moveDirection, float bulletSpeed = 500.0f);

    // Обновление состояния пули
    void update(float deltaTime);

    // Проверка выхода за границы экрана
    bool isOutOfBounds() const;

    // Методы управления состоянием
    void setState(State newState);
    void setPosition(sf::Vector2f pos);
    void setDirection(sf::Vector2f dir);
    void setSpeed(float newSpeed);
    void setType(Type newType);
    void setDamage(float newDamage);
    void setScreenBounds(sf::Vector2u bounds);

    // Геттеры
    sf::Vector2f getPosition() const;
    sf::Vector2f getDirection() const;
    float getSpeed() const;
    Type getType() const;
    State getState() const;
    float getDamage() const;
    sf::Sprite& getSprite();
};