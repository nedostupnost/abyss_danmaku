#include "../libprojectabyss/aLibs.h"
#include <sstream>
#include <array>

using namespace sf;

struct WaveData {
    int enemyCount;
    std::vector<std::pair<EnemyMovementPattern, EnemyShootPattern>> patterns;
};

int main(void)
{
    // Создание окна
    RenderWindow window(VideoMode(800, 600), "Abyss Danmaku");
    window.setFramerateLimit(60);

    // Инициализация систем
    ParticleSystem particleSystem;
    int score = 0;

    // Загрузка ресурсов
    Font font;
    font.loadFromFile("assets/arial.ttf");

    Texture playerTexture;
    playerTexture.loadFromFile("assets/image/reimu.png");

    Texture enemyTexture;
    enemyTexture.loadFromFile("assets/image/enemy.png");

    Texture bulletTexture;
    bulletTexture.loadFromFile("assets/image/bullet.png");

    // Инициализация
    Player player(&playerTexture, window.getSize());
    int enemySpawnTimer = 0;
    int waveTimer = 0;
    int currentWave = 0;
    bool waveInProgress = false;
    int enemiesSpawned = 0;
    bool gameOver = false;
    std::vector<Enemy> enemies;
    std::vector<Bullet> enemyBullets;  // Отдельный вектор для пуль врагов

    // Определяем волны противников
    std::array<WaveData, 5> waves = {{
        // Волна 1: 8 врагов, движущихся синусоидально
        {8, {{PATTERN_SINE, SHOOT_SINGLE}, {PATTERN_SINE, SHOOT_SPREAD}}},
        
        // Волна 2: 12 врагов, движущихся по кругу и зигзагом
        {12, {{PATTERN_CIRCLE, SHOOT_CIRCLE}, {PATTERN_ZIGZAG, SHOOT_AIMED}}},
        
        // Волна 3: 15 врагов со смешанными паттернами
        {15, {{PATTERN_WAVE, SHOOT_SPIRAL}, {PATTERN_STRAIGHT, SHOOT_SPREAD}, {PATTERN_SINE, SHOOT_CIRCLE}}},
        
        // Волна 4: 18 врагов с более сложными паттернами
        {18, {{PATTERN_SPIRAL, SHOOT_CIRCLE}, {PATTERN_CIRCLE, SHOOT_SPIRAL}, {PATTERN_WAVE, SHOOT_AIMED}}},
        
        // Волна 5: 20 врагов с самыми сложными паттернами
        {20, {{PATTERN_SPIRAL, SHOOT_SPIRAL}, {PATTERN_WAVE, SHOOT_CIRCLE}, {PATTERN_CIRCLE, SHOOT_SPREAD}}}
    }};

    // Создаем текст для отображения очков и HP
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(10.f, 40.f);

    // Функция для создания врага с определенным паттерном
    auto spawnEnemy = [&](float x, float y, EnemyMovementPattern movePattern, EnemyShootPattern shootPattern) {
        enemies.push_back(Enemy(&enemyTexture, Vector2f(x, y), movePattern, shootPattern));
    };

    // Игровой цикл
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // Обработка нажатий клавиш для анимации
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case Keyboard::Numpad4:
                    player.set_move_state(3, true); // DIR_LEFT
                    break;
                case Keyboard::Numpad6:
                    player.set_move_state(1, true); // DIR_RIGHT
                    break;
                case Keyboard::Numpad8:
                    player.set_move_state(0, true); // DIR_UP
                    break;
                case Keyboard::Numpad5:
                    player.set_move_state(2, true); // DIR_DOWN
                    break;
                }
            }
            // Обработка отпускания клавиш для анимации
            if (event.type == Event::KeyReleased)
            {
                switch (event.key.code)
                {
                case Keyboard::Numpad4:
                    player.set_move_state(3, false); // DIR_LEFT
                    break;
                case Keyboard::Numpad6:
                    player.set_move_state(1, false); // DIR_RIGHT
                    break;
                case Keyboard::Numpad8:
                    player.set_move_state(0, false); // DIR_UP
                    break;
                case Keyboard::Numpad5:
                    player.set_move_state(2, false); // DIR_DOWN
                    break;
                }
            }
        }

        // Обновление игрока
        player.move();
        player.update();

        // Стрельба игрока
        if (Keyboard::isKeyPressed(Keyboard::Z))
        {
            player.shoot(&bulletTexture);
        }

        // Обновление игры
        if (!gameOver) {
            if (!waveInProgress && enemies.empty() && currentWave < waves.size()) {
                // Начинаем новую волну
                waveInProgress = true;
                enemiesSpawned = 0;
                enemySpawnTimer = 0;
            }

            // Спавн врагов текущей волны
            if (waveInProgress && enemiesSpawned < waves[currentWave].enemyCount) {
                if (enemySpawnTimer >= 30) {  // Спавним врага каждые 30 кадров
                    float x = 100.f + (rand() % 600);  // Случайная позиция по X между 100 и 700
                    
                    // Выбираем случайный паттерн из доступных для текущей волны
                    auto& patterns = waves[currentWave].patterns;
                    auto& pattern = patterns[rand() % patterns.size()];
                    
                    // Спавним врага за пределами экрана сверху
                    spawnEnemy(x, -50.f, pattern.first, pattern.second);
                    enemiesSpawned++;
                    enemySpawnTimer = 0;
                }
                enemySpawnTimer++;
            } else if (waveInProgress && enemies.empty()) {
                // Волна закончилась
                waveInProgress = false;
                currentWave++;
            }

            // Сохраняем пули врагов перед их уничтожением
            for (auto& enemy : enemies) {
                enemyBullets.insert(enemyBullets.end(), enemy.bullets.begin(), enemy.bullets.end());
                enemy.bullets.clear();
            }

            // Проверка столкновений игрока с врагами и их пулями
            if (player.HP > 0) {  // Проверяем только если игрок жив
                sf::FloatRect playerBounds(player.get_left(), player.get_top(),
                                         player.get_width(), player.get_height());
                
                // Проверка столкновений с врагами
                for (const auto& enemy : enemies) {
                    if (playerBounds.intersects(sf::FloatRect(enemy.get_left(), enemy.get_top(),
                                                            enemy.get_width(), enemy.get_height()))) {
                        player.HP--;
                        if (player.HP <= 0) {
                            gameOver = true;
                        }
                        break;
                    }
                }
                
                // Проверка столкновений с пулями врагов
                for (size_t i = 0; i < enemyBullets.size(); i++) {
                    if (playerBounds.intersects(enemyBullets[i].shape.getGlobalBounds())) {
                        player.HP--;
                        enemyBullets.erase(enemyBullets.begin() + i);
                        i--;
                        if (player.HP <= 0) {
                            gameOver = true;
                        }
                    }
                }
            }

            // Обновление пуль врагов
            for (size_t i = 0; i < enemyBullets.size(); i++) {
                enemyBullets[i].update();
                if (enemyBullets[i].isOffscreen(window.getSize())) {
                    enemyBullets.erase(enemyBullets.begin() + i);
                    i--;
                }
            }
        }

        // Обновление частиц
        particleSystem.update(1.0f / 60.0f);

        // Обновление текста очков и HP
        std::stringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());
        
        ss.str("");
        ss << "HP: " << player.HP << "/" << player.HPMax;
        hpText.setString(ss.str());

        // Отрисовка
        window.clear();
        
        // Отрисовка игровых объектов
        player.draw(window);
        for (auto& enemy : enemies) {
            enemy.draw(window);
        }
        for (auto& bullet : enemyBullets) {
            window.draw(bullet.shape);
        }
        
        // Отрисовка UI
        window.draw(scoreText);
        window.draw(hpText);

        // Отображение экрана окончания игры
        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setString("GAME OVER");
            gameOverText.setPosition(
                window.getSize().x / 2.f - gameOverText.getGlobalBounds().width / 2.f,
                window.getSize().y / 2.f - gameOverText.getGlobalBounds().height / 2.f
            );
            window.draw(gameOverText);
        }

        window.display();
    }
    return 0;
}
