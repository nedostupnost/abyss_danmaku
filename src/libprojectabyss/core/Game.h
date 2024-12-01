/**
 * @file Game.h
 * @brief Основной класс управления игровым процессом
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <stack>

#include "../managers/ResourceManager.h"
#include "../managers/EnemyWaveManager.h"
#include "../managers/CollisionManager.h"
#include "../managers/ParticleManager.h"
#include "../entities/Player.h"

/**
 * @brief Перечисление состояний игры
 */
enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    // Основные системные объекты
    sf::RenderWindow window;
    ResourceManager resourceManager;
    
    // Менеджеры подсистем
    EnemyWaveManager enemyWaveManager;
    CollisionManager collisionManager;
    ParticleManager particleManager;
    
    // Игровые сущности
    std::unique_ptr<Player> player;
    
    // Состояние игры
    GameState currentState;
    sf::Clock gameClock;
    float deltaTime;

    // Настройки
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 720;
    const unsigned int FPS_LIMIT = 60;

public:
    /**
     * @brief Конструктор игры
     */
    Game();

    /**
     * @brief Основной игровой цикл
     */
    void run();

private:
    /**
     * @brief Инициализация игровых систем
     */
    void initialize();

    /**
     * @brief Обработка событий
     */
    void processEvents();

    /**
     * @brief Обновление игровой логики
     */
    void update();

    /**
     * @brief Рендеринг игрового мира
     */
    void render();

    /**
     * @brief Обработка состояний игры
     */
    void handleGameStates();

    /**
     * @brief Сброс игры
     */
    void reset();

    /**
     * @brief Пауза игры
     */
    void pause();

    /**
     * @brief Возобновление игры
     */
    void resume();

    /**
     * @brief Завершение игры
     */
    void gameOver();

    /**
     * @brief Обработка столкновений
     */
    void handleCollisions();

    /**
     * @brief Создание частиц
     */
    void createParticleEffects();

    /**
     * @brief Обновление UI
     */
    void updateUI();
};