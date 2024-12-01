/**
 * @file GameState.cpp
 * @brief Реализация класса GameState, управляющего состоянием игры
 * 
 * Этот файл содержит реализацию всех методов класса GameState,
 * который является центральным компонентом игры, управляющим:
 * - игровыми объектами (игрок, враги, пули)
 * - состояниями игры (меню, игра, пауза, game over)
 * - игровой логикой и механикой
 */

#include "GameState.h"

/**
 * @brief Конструктор GameState
 * 
 * Инициализирует все игровые объекты и устанавливает начальные значения.
 * Создает игрока, устанавливает начальное состояние игры и обнуляет счетчики.
 * 
 * @param window Ссылка на окно игры
 */
GameState::GameState(sf::RenderWindow& window)
    : window(window)
    , player(ResourceManager::getInstance().getTexture("player"), window.getSize())
    , currentState(State::PLAYING)
    , score(0)
    , waveInProgress(false)
    , currentWave(0)
    , enemiesSpawned(0)
    , enemySpawnTimer(0)
{
    Logger::log(LogLevel::INFO, "GameState: Инициализация игрового состояния");
    initializeUI();
}

/**
 * @brief Деструктор GameState
 */
GameState::~GameState() {
    Logger::log(LogLevel::INFO, "GameState: Освобождение ресурсов");
}

/**
 * @brief Инициализация UI элементов
 * 
 * Настраивает все текстовые элементы интерфейса
 */
void GameState::initializeUI() {
    sf::Font* font = ResourceManager::getInstance().getFont("main");
    if (!font) {
        Logger::log(LogLevel::ERROR, "GameState: Не удалось загрузить шрифт для UI");
        return;
    }

    // Настройка отображения счета
    scoreText.setFont(*font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    // Настройка отображения здоровья
    hpText.setFont(*font);
    hpText.setCharacterSize(24);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(10.f, 40.f);

    // Настройка текста окончания игры
    gameOverText.setFont(*font);
    gameOverText.setString("GAME OVER\nPress R to restart");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        window.getSize().x / 2.f - gameOverText.getLocalBounds().width / 2.f,
        window.getSize().y / 2.f - gameOverText.getLocalBounds().height / 2.f
    );

    Logger::log(LogLevel::INFO, "GameState: UI элементы инициализированы");
}

/**
 * @brief Обработка пользовательского ввода
 * 
 * Проверяет нажатия клавиш и обновляет состояние игрока.
 * Обрабатывает:
 * - Движение игрока (стрелки)
 * - Стрельбу (клавиша Z)
 * - Другие игровые действия
 */
void GameState::handleInput() {
    if (currentState == State::PLAYING) {
        // Обработка движения игрока
        bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

        player.set_move_state(0, up);    // Движение вверх
        player.set_move_state(1, right); // Движение вправо
        player.set_move_state(2, down);  // Движение вниз
        player.set_move_state(3, left);  // Движение влево

        // Обработка стрельбы
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            player.shoot(ResourceManager::getInstance().getTexture("bullet"));
            Logger::log(LogLevel::DEBUG, "GameState: Игрок произвел выстрел");
        }
    }
    else if (currentState == State::GAME_OVER) {
        // Обработка перезапуска игры
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            reset();
            Logger::log(LogLevel::INFO, "GameState: Игра перезапущена");
        }
    }
}

/**
 * @brief Основной метод обновления игры
 * 
 * Вызывает соответствующие методы обновления в зависимости от текущего состояния игры.
 * Обрабатывает все игровые события и обновляет состояние объектов.
 */
void GameState::update() {
    handleInput();

    switch (currentState) {
        case State::PLAYING:
            updatePlaying();
            break;
        case State::GAME_OVER:
            updateGameOver();
            break;
        default:
            Logger::log(LogLevel::WARNING, "GameState: Неизвестное состояние игры");
            break;
    }
}

/**
 * @brief Обновление игровой логики в активном состоянии
 * 
 * Обновляет позиции всех игровых объектов, проверяет столкновения,
 * обрабатывает игровую логику и условия окончания игры.
 */
void GameState::updatePlaying() {
    // Обновление игрока
    player.update();
    player.move();

    // Обновление врагов
    for (auto& enemy : enemies) {
        enemy.update();
    }

    // Обновление пуль врагов
    for (size_t i = 0; i < enemyBullets.size(); i++) {
        enemyBullets[i].update();
        if (enemyBullets[i].isOffscreen(window.getSize())) {
            enemyBullets.erase(enemyBullets.begin() + i);
            i--;
            Logger::log(LogLevel::DEBUG, "GameState: Удалена пуля за пределами экрана");
        }
    }

    // Обновление визуальных эффектов
    float deltaTime = 1.0f / 60.0f; // Фиксированный временной шаг
    particleSystem.update(deltaTime);

    // Проверка состояния игрока
    if (player.HP <= 0) {
        setState(State::GAME_OVER);
        Logger::log(LogLevel::INFO, "GameState: Игра окончена - игрок побежден");
    }
}

/**
 * @brief Обновление логики в состоянии GAME OVER
 * 
 * Обрабатывает события и анимации после окончания игры.
 * В текущей реализации только обновляет систему частиц.
 */
void GameState::updateGameOver() {
    float deltaTime = 1.0f / 60.0f;
    particleSystem.update(deltaTime);
}

/**
 * @brief Отрисовка всех игровых объектов
 * 
 * Очищает экран и отрисовывает все активные игровые объекты в правильном порядке:
 * 1. Игрок
 * 2. Враги
 * 3. Пули
 * 4. Частицы
 * 5. UI элементы
 */
void GameState::draw() {
    window.clear();

    // Отрисовка игровых объектов
    player.draw(window);
    
    for (const auto& enemy : enemies) {
        const_cast<Enemy&>(enemy).draw(window); // Временное решение
    }
    
    for (const auto& bullet : enemyBullets) {
        window.draw(bullet.shape);
    }

    particleSystem.draw(window);
    drawUI();

    window.display();
}

/**
 * @brief Отрисовка пользовательского интерфейса
 * 
 * Отрисовывает все UI элементы:
 * - Счет игрока
 * - Количество здоровья
 * - Экран окончания игры (если игра окончена)
 */
void GameState::drawUI() {
    // Обновляем текст счета и здоровья
    scoreText.setString("Score: " + std::to_string(score));
    hpText.setString("HP: " + std::to_string(player.HP));

    // Отрисовываем UI элементы
    window.draw(scoreText);
    window.draw(hpText);

    if (currentState == State::GAME_OVER) {
        window.draw(gameOverText);
    }
}

/**
 * @brief Сброс игры в начальное состояние
 * 
 * Сбрасывает все игровые переменные и очищает списки объектов:
 * - Обнуляет счет
 * - Сбрасывает волны
 * - Очищает списки врагов и пуль
 * - Пересоздает игрока
 */
void GameState::reset() {
    score = 0;
    currentWave = 0;
    enemiesSpawned = 0;
    enemySpawnTimer = 0;
    waveInProgress = false;
    enemies.clear();
    enemyBullets.clear();
    
    // Пересоздаем игрока с текстурой из ResourceManager
    player = Player(ResourceManager::getInstance().getTexture("player"), window.getSize());
    currentState = State::PLAYING;
    
    Logger::log(LogLevel::INFO, "GameState: Игра сброшена в начальное состояние");
}

/**
 * @brief Установка нового состояния игры
 */
void GameState::setState(State state) {
    if (currentState != state) {
        currentState = state;
        Logger::log(LogLevel::INFO, "GameState: Состояние изменено на " + std::to_string(static_cast<int>(state)));
    }
}

/**
 * @brief Добавление очков игроку
 * 
 * @param points Количество очков для добавления
 */
void GameState::addScore(int points) {
    score += points;
    Logger::log(LogLevel::DEBUG, "GameState: Добавлено очков: " + std::to_string(points) + 
                                ", всего: " + std::to_string(score));
}

/**
 * @brief Создание нового врага
 * 
 * @param position Позиция появления врага
 * @param movePattern Паттерн движения врага
 * @param shootPattern Паттерн стрельбы врага
 */
void GameState::spawnEnemy(const sf::Vector2f& position, EnemyMovementPattern movePattern, 
                          EnemyShootPattern shootPattern) {
    enemies.push_back(Enemy(ResourceManager::getInstance().getTexture("enemy"), 
                          position, movePattern, shootPattern));
    Logger::log(LogLevel::DEBUG, "GameState: Создан враг на позиции (" + 
                std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
}

/**
 * @brief Добавление пуль врагов в игру
 * 
 * @param bullets Вектор пуль для добавления
 */
void GameState::addEnemyBullets(const std::vector<Bullet>& bullets) {
    enemyBullets.insert(enemyBullets.end(), bullets.begin(), bullets.end());
    Logger::log(LogLevel::DEBUG, "GameState: Добавлено " + 
                std::to_string(bullets.size()) + " вражеских пуль");
}

/**
 * @brief Создание эффекта взрыва
 * 
 * @param position Позиция взрыва
 */
void GameState::createExplosion(const sf::Vector2f& position) {
    particleSystem.addExplosion(position);
    Logger::log(LogLevel::DEBUG, "GameState: Создан взрыв на позиции (" + 
                std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
}