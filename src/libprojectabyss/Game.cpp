#include "Game.h"
#include <cstdlib>

Game::Game(unsigned int width, unsigned int height, const std::string& title) 
    : window(sf::VideoMode(width, height), title), 
      player(&playerTexture, window.getSize()),
      enemySpawnTimer(0) {
    window.setFramerateLimit(60);
    loadResources();
}

void Game::loadResources() {
    font.loadFromFile("assets/arial.ttf");
    playerTexture.loadFromFile("assets/image/reimu.png");
    enemyTexture.loadFromFile("assets/image/marisa.png");
    bulletTexture.loadFromFile("assets/image/bullet.png");
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // Обработка нажатий клавиш для анимации
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Numpad4: player.set_move_state(3, true); break;
                case sf::Keyboard::Numpad6: player.set_move_state(1, true); break;
                case sf::Keyboard::Numpad8: player.set_move_state(0, true); break;
                case sf::Keyboard::Numpad5: player.set_move_state(2, true); break;
            }
        }
        
        // Обработка отпускания клавиш для анимации
        if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::Numpad4: player.set_move_state(3, false); break;
                case sf::Keyboard::Numpad6: player.set_move_state(1, false); break;
                case sf::Keyboard::Numpad8: player.set_move_state(0, false); break;
                case sf::Keyboard::Numpad5: player.set_move_state(2, false); break;
            }
        }
    }
}

void Game::updatePlayer() {
    player.update();
    player.move();

    // Таймер стрельбы
    if (player.shootTimer < 10)
        player.shootTimer++;

    // Стрельба игрока
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.shootTimer >= 10) {
        sf::Vector2f playerPos = player.get_center();
        player.bullets.push_back(Bullet(&bulletTexture, playerPos));
        player.shootTimer = 0;
    }

    // Движение пуль игрока
    for (size_t i = 0; i < player.bullets.size(); i++) {
        player.bullets[i].shape.move(0.f, -10.f);

        // Удаление пули за экраном
        if (player.bullets[i].shape.getPosition().y < 0) {
            player.bullets.erase(player.bullets.begin() + i);
            break;
        }
    }
}

void Game::updateEnemies() {
    for (auto &enemy : enemies) {
        enemy.update();
        enemy.shoot(&bulletTexture);
    }
}

void Game::spawnEnemies() {
    if (enemySpawnTimer < 20)
        enemySpawnTimer++;

    if (enemySpawnTimer >= 20) {
        float randomX = rand() % int(window.getSize().x - 32);
        EnemyMovementPattern pattern = PATTERN_CIRCLE;
        enemies.push_back(Enemy(&enemyTexture, sf::Vector2f(randomX, 50.f), pattern));
        enemySpawnTimer = 0;
    }
}

void Game::checkCollisions() {
    // Проверка коллизии пуль игрока с противниками
    for (size_t i = 0; i < player.bullets.size(); i++) {
        for (size_t k = 0; k < enemies.size(); k++) {
            sf::FloatRect bulletBounds = player.bullets[i].shape.getGlobalBounds();
            sf::FloatRect enemyBounds(enemies[k].get_left(), enemies[k].get_top(),
                                      enemies[k].get_width(), enemies[k].get_height());

            if (bulletBounds.intersects(enemyBounds)) {
                enemies.erase(enemies.begin() + k);
                player.bullets.erase(player.bullets.begin() + i);
                break;
            }
        }
    }

    // Проверка коллизии противников с игроком
    for (size_t i = 0; i < enemies.size(); i++) {
        sf::FloatRect enemyBounds(enemies[i].get_left(), enemies[i].get_top(),
                                  enemies[i].get_width(), enemies[i].get_height());
        sf::FloatRect playerBounds(player.get_left(), player.get_top(),
                                   player.get_width(), player.get_height());

        if (enemyBounds.intersects(playerBounds)) {
            enemies.erase(enemies.begin() + i);
            player.HP--;
            break;
        }

        // Проверка коллизии пуль врага с игроком
        for (size_t j = 0; j < enemies[i].bullets.size(); j++) {
            sf::FloatRect bulletBounds = enemies[i].bullets[j].shape.getGlobalBounds();
            if (bulletBounds.intersects(playerBounds)) {
                enemies[i].bullets.erase(enemies[i].bullets.begin() + j);
                player.HP--;
                break;
            }
        }

        // Удаление противника за экраном
        if (enemies[i].get_top() > window.getSize().y) {
            enemies.erase(enemies.begin() + i);
            break;
        }
    }
}

void Game::render() {
    window.clear();

    // Отрисовка игрока и его пуль
    player.draw(window);
    for (auto &bullet : player.bullets)
        window.draw(bullet.shape);

    // Отрисовка врагов
    for (auto &enemy : enemies)
        enemy.draw(window);

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        updatePlayer();
        updateEnemies();
        spawnEnemies();
        checkCollisions();
        render();
    }
}