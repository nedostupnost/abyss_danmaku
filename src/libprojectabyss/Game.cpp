#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game(unsigned int width, unsigned int height, const std::string &title)
    : window(sf::VideoMode(width, height), title),
      player(&playerTexture, window.getSize()),
      enemySpawnTimer(0),
      currentWave(0),
      enemiesSpawned(0),
      currentWaveDesc(nullptr)
{
    window.setFramerateLimit(60);
    srand(time(nullptr));
    loadResources();
    initializeWaves();
}

void Game::loadResources()
{
    font.loadFromFile("assets/arial.ttf");
    playerTexture.loadFromFile("assets/image/reimu.png");
    enemyTexture.loadFromFile("assets/image/marisa.png");
    bulletTexture.loadFromFile("assets/image/enemies_bullet.png");
    playerBulletTexture.loadFromFile("assets/image/bullet.png");
    hitboxTexture.loadFromFile("assets/image/hit_box_player.png");
    hitboxSprite.setTexture(hitboxTexture);
}

void Game::initializeWaves()
{
    // Волна 1: Простые враги с прямым движением и одиночными выстрелами
    waveQueue.push(createWaveDescription(
        20,  // количество врагов
        {PATTERN_STRAIGHT},
        {BULLET_SINGLE},
        50.f,  // интервал спавна
        2.f    // скорость
    ));

    // Волна 2: Смешанные враги с синусоидальным движением и круговыми выстрелами
    waveQueue.push(createWaveDescription(
        20,
        {PATTERN_SINE, PATTERN_ZIGZAG},
        {BULLET_CIRCLE, BULLET_FAN},
        50.f,
        2.5f,
        2  // HP
    ));

    // Волна 3: Сложные враги с разными паттернами
    waveQueue.push(createWaveDescription(
        20,
        {PATTERN_CIRCLE, PATTERN_EIGHT, PATTERN_SINE},
        {BULLET_SPIRAL, BULLET_WAVE, BULLET_AIMED},
        50.f,
        3.f,
        3
    ));
}

WaveDescription Game::createWaveDescription(
    int enemyCount,
    const std::vector<EnemyMovementPattern>& movePatterns,
    const std::vector<BulletPattern>& shootPatterns,
    float spawnInterval,
    float enemySpeed,
    int enemyHP)
{
    WaveDescription wave;
    wave.enemyCount = enemyCount;
    wave.movePatterns = movePatterns;
    wave.shootPatterns = shootPatterns;
    wave.spawnInterval = spawnInterval;
    wave.enemySpeed = enemySpeed;
    wave.enemyHP = enemyHP;
    return wave;
}

void Game::updateWaveSystem()
{
    // Если текущая волна закончилась и очередь не пуста
    if (enemies.empty() && enemiesSpawned >= (currentWaveDesc ? currentWaveDesc->enemyCount : 0))
    {
        if (!waveQueue.empty())
        {
            delete currentWaveDesc;
            currentWaveDesc = new WaveDescription(waveQueue.front());
            waveQueue.pop();
            currentWave++;
            enemiesSpawned = 0;
            enemySpawnTimer = 0;
        }
    }
}

void Game::spawnEnemies()
{
    // Увеличиваем таймер спавна
    enemySpawnTimer++;

    // Спавним нового врага каждые 60 кадров (примерно 1 секунда)
    if (enemySpawnTimer >= 15)
    {
        // Рандомизируем паттерны движения и стрельбы
        EnemyMovementPattern movePattern = static_cast<EnemyMovementPattern>(rand() % NUM_MOVEMENT_PATTERNS);
        BulletPattern shootPattern = static_cast<BulletPattern>(rand() % NUM_BULLET_PATTERNS);

        // Создаем нового врага
        enemies.emplace_back(&enemyTexture, Vector2f(rand() % window.getSize().x, 0), movePattern, shootPattern);

        // Сбрасываем таймер спавна
        enemySpawnTimer = 0;
    }
}

void Game::updateEnemies()
{
    Vector2f playerPos = player.get_center();
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->update(playerPos, &bulletTexture);

        // Удаляем врагов, вышедших за пределы экрана
        if (it->get_top() > window.getSize().y) {
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateBullets()
{
    // Обновление пуль игрока
    player.updateBullets();

    // Обновление пуль врагов
    for (auto& enemy : enemies) {
        enemy.updateBullets();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        // Обработка нажатий клавиш для анимации
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Numpad4:
                player.set_move_state(3, true);
                break;
            case sf::Keyboard::Numpad6:
                player.set_move_state(1, true);
                break;
            case sf::Keyboard::Numpad8:
                player.set_move_state(0, true);
                break;
            case sf::Keyboard::Numpad5:
                player.set_move_state(2, true);
                break;
            }
        }

        // Обработка отпускания клавиш для анимации
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Numpad4:
                player.set_move_state(3, false);
                break;
            case sf::Keyboard::Numpad6:
                player.set_move_state(1, false);
                break;
            case sf::Keyboard::Numpad8:
                player.set_move_state(0, false);
                break;
            case sf::Keyboard::Numpad5:
                player.set_move_state(2, false);
                break;
            }
        }
    }
}

void Game::updatePlayer()
{
    player.update();

    // Стрельба игрока
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.getShootTimer() >= 5)
    {
        player.shoot(&playerBulletTexture);
        player.setShootTimer(0);
    }
    
    // Увеличиваем таймер стрельбы
    if (player.getShootTimer() < 5)
        player.incrementShootTimer();
}

void Game::checkCollisions()
{
    // Точное определение хитбокса игрока
    sf::FloatRect hitboxBounds(
        player.get_left() + (player.get_width() - 3) / 2,
        player.get_top() + (player.get_height() - 3) / 2,
        3, 3
    );

    // Проверка коллизии пуль игрока с противниками
    auto& playerBullets = player.getBullets();
    for (size_t i = 0; i < playerBullets.size();)
    {
        bool bulletDestroyed = false;
        for (size_t k = 0; k < enemies.size();)
        {
            sf::FloatRect bulletBounds = playerBullets[i].shape.getGlobalBounds();
            sf::FloatRect enemyBounds(enemies[k].get_left(), enemies[k].get_top(),
                                    enemies[k].get_width(), enemies[k].get_height());

            if (bulletBounds.intersects(enemyBounds))
            {
                // Уменьшаем HP врага
                enemies[k].takeDamage(1);
                
                // Если HP врага достиг нуля, удаляем его
                if (!enemies[k].isAlive()) {
                    enemies.erase(enemies.begin() + k);
                } else {
                    ++k;
                }
                
                playerBullets.erase(playerBullets.begin() + i);
                bulletDestroyed = true;
                break;
            }
            else
            {
                ++k;
            }
        }
        
        if (!bulletDestroyed)
        {
            ++i;
        }
    }

    // Проверка коллизии противников с игроком
    for (size_t i = 0; i < enemies.size();)
    {
        sf::FloatRect enemyBounds(enemies[i].get_left(), enemies[i].get_top(),
                                enemies[i].get_width(), enemies[i].get_height());

        if (enemyBounds.intersects(hitboxBounds))
        {
            enemies.erase(enemies.begin() + i);
            player.takeDamage(1);
            continue;
        }
        ++i;
    }

    // Проверка коллизии пуль врагов с игроком
    for (auto& enemy : enemies) {
        auto& enemyBullets = enemy.getBullets();
        for (size_t j = 0; j < enemyBullets.size();)
        {
            sf::FloatRect bulletBounds = enemyBullets[j].shape.getGlobalBounds();
            
            // Проверка столкновения с хитбоксом
            if (bulletBounds.intersects(hitboxBounds))
            {
                enemyBullets.erase(enemyBullets.begin() + j);
                player.takeDamage(1);
            }
            else
            {
                ++j;
            }
        }
    }
}

void Game::render()
{
    window.clear();

    // Отрисовка игрока и врагов
    player.draw(window);
    for (auto &enemy : enemies) {
        enemy.draw(window);
    }

    // Отрисовка хитбокса
    hitboxSprite.setPosition(
        player.get_left() + (player.get_width() - 30) / 2,
        player.get_top() + (player.get_height() - 18) / 2);
    window.draw(hitboxSprite);

    window.display();
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        
        updatePlayer();
        updateWaveSystem();
        spawnEnemies();
        updateEnemies();
        updateBullets();
        checkCollisions();
        
        render();
    }
}