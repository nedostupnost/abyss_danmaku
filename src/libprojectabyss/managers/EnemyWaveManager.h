/**
 * @file EnemyWaveManager.h
 * @brief Менеджер волн противников
 */
#pragma once

#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>

#include "../entities/Enemy.h"
#include "../managers/ResourceManager.h"

/**
 * @brief Уровень сложности волны противников
 */
enum class WaveDifficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME
};

/**
 * @brief Структура, описывающая волну противников
 */
struct EnemyWave {
    std::vector<std::unique_ptr<Enemy>> enemies;  ///< Список противников в волне
    WaveDifficulty difficulty;                    ///< Сложность волны
    float spawnInterval;                          ///< Интервал между спауном противников
    int totalEnemies;                             ///< Общее количество противников в волне
};

/**
 * @brief Менеджер управления волнами противников
 * 
 * Отвечает за:
 * - Генерацию волн противников
 * - Управление сложностью
 * - Спаун противников
 * - Обновление и отрисовку
 */
class EnemyWaveManager {
private:
    /// Ресурс-менеджер для загрузки текстур
    ResourceManager& resourceManager;

    /// Текущая активная волна
    std::unique_ptr<EnemyWave> currentWave;

    /// Окно для рендеринга
    sf::RenderWindow& window;

    /// Генератор случайных чисел
    std::mt19937 randomGenerator;

    /// Текущий уровень игры
    int currentLevel;

    /// Таймер для спауна противников
    float spawnTimer;

public:
    /**
     * @brief Конструктор менеджера волн
     * 
     * @param resourceMgr Ссылка на менеджер ресурсов
     * @param gameWindow Ссылка на окно рендеринга
     */
    EnemyWaveManager(ResourceManager& resourceMgr, sf::RenderWindow& gameWindow);

    /**
     * @brief Обновление состояния волн противников
     * 
     * @param deltaTime Время между кадрами
     */
    void update(float deltaTime);

    /**
     * @brief Отрисовка противников
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Получение количества живых противников
     * 
     * @return int Количество живых противников
     */
    int getAliveEnemyCount() const;

private:
    /**
     * @brief Генерация новой волны противников
     * 
     * @param difficulty Сложность волны
     */
    void generateWave(WaveDifficulty difficulty);

    /**
     * @brief Спаун противника в текущей волне
     */
    void spawnEnemy();

    /**
     * @brief Выбор типа противника для спауна
     * 
     * @return EnemyMovementPattern Паттерн движения противника
     */
    EnemyMovementPattern selectEnemyType();

    /**
     * @brief Расчет сложности волны на основе текущего уровня
     * 
     * @return WaveDifficulty Сложность волны
     */
    WaveDifficulty calculateWaveDifficulty();
};