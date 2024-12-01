/**
 * @file Game.cpp
 * @brief Реализация основного класса управления игровым процессом
 */
#include "Game.h"

Game::Game() : 
    window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Abyss Danmaku"),
    enemyWaveManager(resourceManager, window),
    collisionManager(
        *player, 
        enemyWaveManager.getCurrentWave()->enemies, 
        currentState
    ),
    currentState(GameState::MAIN_MENU),
    deltaTime(0.0f)
{
    initialize();
}

void Game::initialize() {
    // Настройка окна
    window.setFramerateLimit(FPS_LIMIT);

    // Загрузка ресурсов
    resourceManager.loadTextures();
    resourceManager.loadSounds();

    // Создание игрока
    player = std::make_unique<Player>(
        resourceManager.getTexture("player"),
        sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.9f)
    );

    // Инициализация менеджеров
    enemyWaveManager.initialize();
}

void Game::run() {
    while (window.isOpen()) {
        // Обновление времени между кадрами
        deltaTime = gameClock.restart().asSeconds();

        processEvents();
        
        if (currentState == GameState::PLAYING) {
            update();
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Обработка ввода в зависимости от состояния игры
        switch (currentState) {
            case GameState::MAIN_MENU:
                handleMainMenuInput(event);
                break;
            case GameState::PLAYING:
                handleGameplayInput(event);
                break;
            case GameState::PAUSED:
                handlePauseInput(event);
                break;
            case GameState::GAME_OVER:
                handleGameOverInput(event);
                break;
        }
    }
}

void Game::update() {
    // Обновление игровых систем
    player->update(deltaTime);
    enemyWaveManager.update(deltaTime);
    
    // Обработка столкновений
    collisionManager.update(deltaTime);
    
    // Обновление частиц
    particleManager.update(deltaTime);
    
    // Проверка условий завершения игры
    if (player->isDead()) {
        gameOver();
    }

    // Создание визуальных эффектов
    createParticleEffects();/**
 * @file Game.cpp
 * @brief Реализация основного игрового класса
 * 
 * Содержит логику игрового цикла, обработки событий 
 * и управления игровыми подсистемами
 */

#include "Game.h"

/**
 * @brief Конструктор игры
 * 
 * Инициализирует основные компоненты игры:
 * - Окно с фиксированным размером и заголовком
 * - Игрового персонажа с загруженной текстурой
 * - Параметры рендеринга
 */
Game::Game() : 
    // Создаем окно размером 800x600 с заголовком
    window(sf::VideoMode(800, 600), "Abyss Danmaku"),
    
    // Инициализируем игрока с текстурой из ResourceManager
    player(resourceManager.getTexture("player"), window.getSize()) 
{
    // Устанавливаем ограничение кадров для стабильной производительности
    window.setFramerateLimit(60);
}

/**
 * @brief Основной метод игрового цикла
 * 
 * Реализует непрерывный цикл игры:
 * 1. Измерение времени между кадрами
 * 2. Обработка событий
 * 3. Обновление игровой логики
 * 4. Отрисовка кадра
 */
void Game::run() {
    // Создаем часы для отслеживания времени между кадрами
    sf::Clock clock;

    // Основной игровой цикл
    while (window.isOpen()) {
        // Получаем время, прошедшее с последнего кадра
        // Используется для плавной анимации и физики
        float deltaTime = clock.restart().asSeconds();

        // Обработка входящих событий (нажатия клавиш, закрытие окна)
        processEvents();

        // Обновление игровой логики
        update(deltaTime);

        // Отрисовка игровых объектов
        render();
    }
}

/**
 * @brief Обработка событий окна и пользовательского ввода
 * 
 * Последовательно обрабатывает все накопившиеся события:
 * - Закрытие окна
 * - Нажатия и отпускания клавиш
 * - Передача событий соответствующим игровым объектам
 */
void Game::processEvents() {
    // Объект события для получения входящих событий
    sf::Event event;

    // Цикл обработки всех накопившихся событий
    while (window.pollEvent(event)) {
        // Проверка события закрытия окна
        if (event.type == sf::Event::Closed)
            window.close();
        
        // Передача события управления игроку
        // Внутри метода будет обработка нажатий клавиш
        player.handleInput(event);
    }
}

/**
 * @brief Обновление игровой логики
 * 
 * @param deltaTime Время, прошедшее с последнего кадра
 * 
 * Последовательно обновляет состояния игровых объектов:
 * - Состояние игрока
 * - Волны противников
 * - Проверка столкновений
 */
void Game::update(float deltaTime) {
    // Обновление состояния игрока
    player.update(deltaTime);

    // Обновление волн противников
    enemyWaveManager.update(deltaTime);

    // Проверка столкновений между объектами
    collisionManager.update(deltaTime);
}

/**
 * @brief Отрисовка игровых объектов
 * 
 * Последовательность отрисовки кадра:
 * 1. Очистка экрана
 * 2. Отрисовка игрока
 * 3. Отрисовка волн противников
 * 4. Отображение готового кадра
 */
void Game::render() {
    // Очистка экрана перед новым кадром
    window.clear();

    // Отрисовка игрока
    player.render(window);

    // Отрисовка волн противников
    enemyWaveManager.render(window);

    // Отображение готового кадра
    window.display();
}
    // Рендеринг в зависимости от состояния игры
    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            break;
        case GameState::PLAYING:
            renderGameplay();
            break;
        case GameState::PAUSED:
            renderPauseMenu();
            break;
        case GameState::GAME_OVER:
            renderGameOverScreen();
            break;
    }

    window.display();
}

void Game::handleGameStates() {
    // Логика переключения состояний игры
    switch (currentState) {
        case GameState::MAIN_MENU:
            if (startGameRequested) {
                currentState = GameState::PLAYING;
                reset();
            }
            break;
        case GameState::PLAYING:
            if (pauseRequested) {
                currentState = GameState::PAUSED;
                pause();
            }
            break;
        case GameState::PAUSED:
            if (resumeRequested) {
                currentState = GameState::PLAYING;
                resume();
            }
            break;
        case GameState::GAME_OVER:
            if (restartRequested) {
                currentState = GameState::MAIN_MENU;
                reset();
            }
            break;
    }
}

void Game::reset() {
    // Сброс игровых систем
    player->reset();
    enemyWaveManager.reset();
    particleManager = ParticleManager();
    
    // Сброс счета, жизней и т.д.
    gameScore = 0;
    playerLives = 3;
}

void Game::pause() {
    // Приостановка игровых систем
    gameClock.pause();
}

void Game::resume() {
    // Возобновление игровых систем
    gameClock.restart();
}

void Game::gameOver() {
    currentState = GameState::GAME_OVER;
    // Логика завершения игры
}

void Game::handleCollisions() {
    // Обработка событий столкновений
    for (const auto& event : collisionManager.getCollisionEvents()) {
        switch (event.type) {
            case CollisionType::PLAYER_ENEMY:
                player->takeDamage(event.damage);
                break;
            case CollisionType::ENEMY_BULLET:
                enemyWaveManager.removeEnemy(event.position);
                gameScore += 100;
                break;
            // Другие типы столкновений
        }
    }
}

void Game::createParticleEffects() {
    // Создание частиц для различных событий
    if (enemyDestroyed) {
        particleManager.createParticles(
            enemyPosition, 
            ParticleType::EXPLOSION, 
            50
        );
    }

    if (playerHit) {
        particleManager.createParticles(
            player->getPosition(), 
            ParticleType::DAMAGE, 
            20
        );
    }
}

void Game::updateUI() {
    // Обновление элементов пользовательского интерфейса
    scoreText.setString("Score: " + std::to_string(gameScore));
    livesText.setString("Lives: " + std::to_string(playerLives));
}

// Приватные методы обработки ввода
void Game::handleMainMenuInput(const sf::Event& event) {
    // Обработка ввода в главном меню
}

void Game::handleGameplayInput(const sf::Event& event) {
    // Обработка ввода во время игры
    player->handleInput(event);
}

void Game::handlePauseInput(const sf::Event& event) {
    // Обработка ввода в режиме паузы
}

void Game::handleGameOverInput(const sf::Event& event) {
    // Обработка ввода на экране окончания игры
}

// Методы рендеринга
void Game::renderMainMenu() {
    // Рендеринг главного меню
}

void Game::renderGameplay() {
    // Рендеринг игрового процесса
    player->render(window);
    enemyWaveManager.render(window);
    particleManager.render(window);
    renderUI();
}

void Game::renderPauseMenu() {
    // Рендеринг меню паузы
}

void Game::renderGameOverScreen() {
    // Рендеринг экрана окончания игры
}

void Game::renderUI() {
    // Рендеринг элементов пользовательского интерфейса
    window.draw(scoreText);
    window.draw(livesText);
}