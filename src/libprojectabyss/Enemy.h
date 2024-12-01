#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "AnimatedSprite.h"
#include "Bullet.h"
#include "Player.h"

/**
 * @brief Паттерны движения врагов
 */
enum EnemyMovementPattern {
    PATTERN_STRAIGHT,  ///< Прямое движение вниз
    PATTERN_SINE,      ///< Синусоидальное движение
    PATTERN_ZIGZAG,    ///< Зигзагообразное движение
    PATTERN_CIRCLE,    ///< Круговое движение
    PATTERN_WAVE,      ///< Волнообразное движение
    PATTERN_SPIRAL     ///< Спиральное движение
};

/**
 * @brief Паттерны стрельбы врагов
 */
enum EnemyShootPattern {
    SHOOT_SINGLE,     ///< Одиночные выстрелы
    SHOOT_SPREAD,     ///< Веерная стрельба
    SHOOT_CIRCLE,     ///< Круговая стрельба
    SHOOT_SPIRAL,     ///< Спиральная стрельба
    SHOOT_AIMED       ///< Прицельная стрельба в игрока
};

/**
 * @brief Класс врага
 * 
 * Представляет собой вражеский корабль с различными паттернами
 * движения и стрельбы. Управляет своим состоянием, движением и атаками.
 */
class Enemy {
private:
    AnimatedSprite sprite;              ///< Спрайт врага
    EnemyMovementPattern movePattern;   ///< Текущий паттерн движения
    EnemyShootPattern shootPattern;     ///< Текущий паттерн стрельбы
    float rotationAngle;                ///< Угол поворота для спиральной стрельбы
    float movementTimer;                ///< Таймер для паттернов движения
    float baseX;                        ///< Базовая позиция X
    float baseY;                        ///< Базовая позиция Y
    float speed;                        ///< Скорость движения

public:
    int HP;                             ///< Текущее здоровье
    int HPMax;                          ///< Максимальное здоровье
    int damageTimer;                    ///< Таймер неуязвимости после получения урона
    int shootTimer;                     ///< Таймер между выстрелами
    std::vector<Bullet> bullets;        ///< Пули, выпущенные врагом

    /**
     * @brief Конструктор врага
     * @param texture Указатель на текстуру
     * @param pos Начальная позиция
     * @param movePattern Паттерн движения
     * @param shootPattern Паттерн стрельбы
     */
    Enemy(sf::Texture* texture, sf::Vector2f pos, 
          EnemyMovementPattern movePattern = PATTERN_STRAIGHT, 
          EnemyShootPattern shootPattern = SHOOT_SINGLE);

    /**
     * @brief Обновление состояния врага
     */
    void update();

    /**
     * @brief Отрисовка врага
     * @param target Цель для отрисовки
     */
    void draw(sf::RenderTarget& target) const;  // Теперь метод константный

    /**
     * @brief Выстрел врага
     * @param bulletTexture Текстура пули
     * @param playerPos Позиция игрока для прицельной стрельбы
     */
    void shoot(sf::Texture* bulletTexture, const sf::Vector2f& playerPos);

    /**
     * @brief Обновление движения врага
     */
    void updateMovement();

    /**
     * @brief Установка позиции врага
     * @param pos Новая позиция
     */
    void set_position(const sf::Vector2f& pos);

    /**
     * @brief Получение текущей позиции
     * @return Текущая позиция врага
     */
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    /**
     * @brief Получение размеров спрайта
     * @return Размеры спрайта врага
     */
    sf::Vector2f getSize() const { return sprite.getSize(); }

    /**
     * @brief Получение границ спрайта
     * @return Прямоугольник границ врага
     */
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    // Методы для доступа к позиции и размерам
    double get_left() const { return sprite.get_left(); }
    double get_right() const { return sprite.get_right(); }
    double get_top() const { return sprite.get_top(); }
    double get_bottom() const { return sprite.get_bottom(); }
    double get_width() const { return sprite.get_width(); }
    double get_height() const { return sprite.get_height(); }
    sf::Vector2f get_center() const { return sprite.get_center(); }
};
