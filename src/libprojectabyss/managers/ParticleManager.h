/**
 * @file ParticleManager.h
 * @brief Менеджер систем частиц для визуальных эффектов
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

/**
 * @brief Типы частиц в игре
 */
enum class ParticleType {
    EXPLOSION,      ///< Взрыв при уничтожении врага
    BULLET_TRAIL,   ///< След от пули
    DAMAGE,         ///< Эффект повреждения
    POWERUP,        ///< Эффект подбора бонуса
    GRAZE           ///< Эффект близкого прохождения
};

/**
 * @brief Структура частицы
 */
struct Particle {
    sf::CircleShape shape;     ///< Форма частицы
    sf::Vector2f velocity;     ///< Вектор движения
    float lifetime;            ///< Время жизни частицы
    ParticleType type;         ///< Тип частицы
};

/**
 * @brief Менеджер управления системами частиц
 * 
 * Отвечает за:
 * - Создание частиц
 * - Обновление и анимацию частиц
 * - Рендеринг частиц
 */
class ParticleManager {
private:
    /// Список активных частиц
    std::vector<Particle> particles;

    /// Генератор случайных чисел
    std::mt19937 randomGenerator;

    /// Максимальное количество частиц
    const size_t MAX_PARTICLES = 1000;

public:
    /**
     * @brief Создание частиц в указанной позиции
     * 
     * @param position Позиция создания частиц
     * @param type Тип создаваемых частиц
     * @param count Количество частиц
     */
    void createParticles(
        const sf::Vector2f& position, 
        ParticleType type, 
        size_t count
    );

    /**
     * @brief Обновление состояния частиц
     * 
     * @param deltaTime Время между кадрами
     */
    void update(float deltaTime);

    /**
     * @brief Отрисовка частиц
     * 
     * @param window Окно для рендеринга
     */
    void render(sf::RenderWindow& window);

private:
    /**
     * @brief Создание частиц взрыва
     * 
     * @param position Позиция взрыва
     * @param count Количество частиц
     */
    void createExplosionParticles(
        const sf::Vector2f& position, 
        size_t count
    );

    /**
     * @brief Создание следа от пули
     * 
     * @param position Позиция создания следа
     * @param count Количество частиц
     */
    void createBulletTrailParticles(
        const sf::Vector2f& position, 
        size_t count
    );

    /**
     * @brief Очистка устаревших частиц
     */
    void cleanupParticles();
};