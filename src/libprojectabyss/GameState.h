#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "ParticleSystem.h"
#include "Logger.h"
#include "ResourceManager.h"

/**
 * @brief Перечисление возможных состояний игры
 * 
 * MENU - главное меню игры
 * PLAYING - активный игровой процесс
 * PAUSED - игра на паузе
 * GAME_OVER - игра окончена
 */

enum class State {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

/**
 * @brief Основной класс управления состоянием игры
 * 
 * Этот класс является центральным компонентом игры, который:
 * - Управляет игровыми объектами (игрок, враги, пули)
 * - Обрабатывает игровую логику
 * - Контролирует состояния игры
 * - Управляет очками и прогрессом
 */
class GameState {
private:
    // === Игровые объекты ===
    sf::RenderWindow& window;        ///< Ссылка на окно игры
    Player player;                   ///< Объект игрока
    std::vector<Enemy> enemies;      ///< Список активных врагов
    std::vector<Bullet> enemyBullets;///< Список пуль врагов
    ParticleSystem particleSystem;   ///< Система частиц для эффектов

    // === Состояние игры ===
    State currentState;              ///< Текущее состояние игры
    int score;                       ///< Текущие очки игрока
    bool waveInProgress;             ///< Флаг активной волны врагов
    int currentWave;                 ///< Номер текущей волны
    int enemiesSpawned;             ///< Количество созданных врагов в текущей волне
    int enemySpawnTimer;            ///< Таймер для спавна врагов

    // === UI элементы ===
    sf::Text scoreText;             ///< Текст для отображения очков
    sf::Text hpText;                ///< Текст для отображения здоровья
    sf::Text gameOverText;          ///< Текст для экрана окончания игры

    /**
     * @brief Инициализация UI элементов
     */
    void initializeUI();

    /**
     * @brief Обработка пользовательского ввода
     */
    void handleInput();

    /**
     * @brief Обновление игровой логики в состоянии PLAYING
     */
    void updatePlaying();

    /**
     * @brief Обновление игровой логики в состоянии GAME_OVER
     */
    void updateGameOver();

    /**
     * @brief Отрисовка UI элементов
     */
    void drawUI();

public:
    /**
     * @brief Конструктор GameState
     * @param window Ссылка на окно игры
     * @param playerTexture Текстура игрока
     */
    explicit GameState(sf::RenderWindow& window);
    
    /**
     * @brief Деструктор GameState
     */
    ~GameState();

    // === Основные методы управления игрой ===
    /**
     * @brief Обновление игровой логики
     */
    void update();

    /**
     * @brief Отрисовка всех игровых объектов
     */
    void draw();

    /**
     * @brief Сброс игры в начальное состояние
     */
    void reset();

    // === Геттеры ===
    /**
     * @brief Получить текущее состояние игры
     * @return Текущее состояние
     */
    State getState() const { return currentState; }

    /**
     * @brief Получить текущие очки
     * @return Количество очков
     */
    int getScore() const { return score; }

    /**
     * @brief Проверить, окончена ли игра
     * @return true если игра окончена
     */
    bool isGameOver() const { return currentState == State::GAME_OVER; }

    // === Сеттеры ===
    /**
     * @brief Установить состояние игры
     * @param state Новое состояние
     */
    void setState(State state);

    /**
     * @brief Добавить очки
     * @param points Количество очков для добавления
     */
    void addScore(int points);

    // === Методы для взаимодействия с другими менеджерами ===
    /**
     * @brief Создать нового врага
     * @param position Позиция появления
     * @param movePattern Паттерн движения
     * @param shootPattern Паттерн стрельбы
     */
    void spawnEnemy(const sf::Vector2f& position, EnemyMovementPattern movePattern, EnemyShootPattern shootPattern);

    /**
     * @brief Добавить пули врагов
     * @param bullets Вектор пуль для добавления
     */
    void addEnemyBullets(const std::vector<Bullet>& bullets);

    /**
     * @brief Создать эффект взрыва
     * @param position Позиция взрыва
     */
    void createExplosion(const sf::Vector2f& position);

    // === Методы доступа к игровым объектам ===
    /**
     * @brief Получить объект игрока
     * @return Константная ссылка на игрока
     */
    const Player& getPlayer() const { return player; }

    /**
     * @brief Получить список врагов
     * @return Константная ссылка на вектор врагов
     */
    const std::vector<Enemy>& getEnemies() const { return enemies; }

    /**
     * @brief Получить список пуль врагов
     * @return Константная ссылка на вектор пуль
     */
    const std::vector<Bullet>& getEnemyBullets() const { return enemyBullets; }
};