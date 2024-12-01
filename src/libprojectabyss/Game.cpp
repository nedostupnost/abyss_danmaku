#include "Game.h"
#include <cstdlib>

Game::Game(unsigned int width, unsigned int height, const std::string &title)
    : window(sf::VideoMode(width, height), title),
      player(&playerTexture, window.getSize()),
      enemySpawnTimer(0)
{
    window.setFramerateLimit(60);
    loadResources();
}

void Game::loadResources()
{
    font.loadFromFile("assets/arial.ttf");
    playerTexture.loadFromFile("assets/image/reimu.png");
    enemyTexture.loadFromFile("assets/image/marisa.png");
    bulletTexture.loadFromFile("assets/image/bullet.png");
    hitboxTexture.loadFromFile("assets/image/hit_box_player.png");
    hitboxSprite.setTexture(hitboxTexture);
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
    player.move();

    // Таймер стрельбы
    if (player.shootTimer < 10)
        player.shootTimer++;

    // Стрельба игрока
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.shootTimer >= 10)
    {
        sf::Vector2f playerPos = player.get_center();
        player.bullets.push_back(Bullet(&bulletTexture, playerPos));
        player.shootTimer = 0;
    }

    // Движение пуль игрока
    for (size_t i = 0; i < player.bullets.size();)
    {
        player.bullets[i].shape.move(0.f, -10.f);

        // Удаление пули за экраном
        if (player.bullets[i].shape.getPosition().y < 0)
        {
            player.bullets.erase(player.bullets.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void Game::updateEnemies()
{
    for (auto it = enemies.begin(); it != enemies.end(); ++it)
    {
        it->update();
        it->shoot(&bulletTexture);

        // Добавляем пули врагов в общий вектор
        for (auto &bullet : it->bullets)
        {
            enemyBullets.push_back(bullet);
        }
        it->bullets.clear(); // Очищаем локальный вектор пуль врага
    }
}

void Game::updateBullets()
{
    // Обновление пуль игрока
    for (size_t i = 0; i < player.bullets.size();)
    {
        player.bullets[i].shape.move(0.f, -10.f);

        // Удаление пули за экраном
        if (player.bullets[i].shape.getPosition().y < 0)
        {
            player.bullets.erase(player.bullets.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    // Обновление пуль врагов
    for (size_t i = 0; i < enemyBullets.size();)
    {
        enemyBullets[i].shape.move(0.f, 10.f);

        // Удаление пули за экраном
        if (enemyBullets[i].shape.getPosition().y > window.getSize().y ||
            enemyBullets[i].shape.getPosition().y < 0 ||
            enemyBullets[i].shape.getPosition().x < 0 ||
            enemyBullets[i].shape.getPosition().x > window.getSize().x)
        {
            enemyBullets.erase(enemyBullets.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void Game::spawnEnemies()
{
    if (enemySpawnTimer < 20)
        enemySpawnTimer++;

    if (enemySpawnTimer >= 20)
    {
        float randomX = rand() % int(window.getSize().x - 32);
        EnemyMovementPattern pattern = PATTERN_CIRCLE;
        enemies.push_back(Enemy(&enemyTexture, sf::Vector2f(randomX, 50.f), pattern));
        enemySpawnTimer = 0;
    }
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
    for (size_t i = 0; i < player.bullets.size();)
    {
        bool bulletDestroyed = false;
        for (size_t k = 0; k < enemies.size();)
        {
            sf::FloatRect bulletBounds = player.bullets[i].shape.getGlobalBounds();
            sf::FloatRect enemyBounds(enemies[k].get_left(), enemies[k].get_top(),
                                      enemies[k].get_width(), enemies[k].get_height());

            if (bulletBounds.intersects(enemyBounds))
            {
                enemies.erase(enemies.begin() + k);
                player.bullets.erase(player.bullets.begin() + i);
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
            player.HP--;
            continue;
        }

        // Проверка коллизии пуль врага с игроком
        for (size_t j = 0; j < enemyBullets.size();)
        {
            sf::FloatRect bulletBounds = enemyBullets[j].shape.getGlobalBounds();
            
            // Проверка столкновения с хитбоксом
            if (bulletBounds.intersects(hitboxBounds))
            {
                enemyBullets.erase(enemyBullets.begin() + j);
                player.HP--;
                break;
            }
            else
            {
                ++j;
            }
        }

        ++i;
    }
}

void Game::render()
{
    window.clear();

    // Отрисовка игрока и его пуль
    player.draw(window);
    for (auto &bullet : player.bullets)
        window.draw(bullet.shape);

    // Отрисовка врагов и их пуль
    for (auto &enemy : enemies)
        enemy.draw(window);

    for (auto &bullet : enemyBullets)
        window.draw(bullet.shape);

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
        updateEnemies();
        updateBullets();
        spawnEnemies();
        checkCollisions();
        render();
    }
}