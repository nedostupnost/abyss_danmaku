#include "../libprojectabyss/aLibs.h"

using namespace sf;

int main(void)
{
    // Создание окна
    RenderWindow window(VideoMode(800, 600), "Abyss Danmaku");
    window.setFramerateLimit(60);

    // Загрузка ресурсов
    Font font;
    font.loadFromFile("assets/arial.ttf");

    Texture playerTexture;
    playerTexture.loadFromFile("assets/image/reimu.png"); // спрайтлист персонажа

    Texture enemyTexture;
    enemyTexture.loadFromFile("assets/image/marisa.png"); // спрайтлист противника

    Texture bulletTexture;
    bulletTexture.loadFromFile("assets/image/bullet.png"); // просто спрайт пули

    // Инициализация
    Player player(&playerTexture, window.getSize());
    int enemySpawnTimer = 0;
    int waveTimer = 0;  // Таймер для волн врагов
    int currentWave = 0;  // Текущая волна
    std::vector<Enemy> enemies;

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

        // Обновление анимации
        player.update();
        player.move();

        // Обновление врагов
        for (auto &enemy : enemies)
        {
            enemy.update();
            enemy.shoot(&bulletTexture, player.get_center()); // Передаем позицию игрока для прицельной стрельбы
        }

        // Таймер стрельбы
        if (player.shootTimer < 10)
        {
            player.shootTimer++;
        }

        // Стрельба игрока
        if (Keyboard::isKeyPressed(Keyboard::Z) && player.shootTimer >= 10)
        {
            Vector2f playerPos = player.get_center();
            player.bullets.push_back(Bullet(&bulletTexture, playerPos));
            player.shootTimer = 0;
        }

        // Движение и обработка пуль игрока
        for (size_t i = 0; i < player.bullets.size(); i++)
        {
            player.bullets[i].shape.move(0.f, -10.f);

            // Удаление пули за экраном
            if (player.bullets[i].shape.getPosition().y < 0)
            {
                player.bullets.erase(player.bullets.begin() + i);
                break;
            }

            // Проверка коллизии пуль игрока с противниками
            for (size_t k = 0; k < enemies.size(); k++)
            {
                // Создаем прямоугольники для проверки коллизий
                sf::FloatRect bulletBounds = player.bullets[i].shape.getGlobalBounds();
                sf::FloatRect enemyBounds(enemies[k].get_left(), enemies[k].get_top(),
                                          enemies[k].get_width(), enemies[k].get_height());

                if (bulletBounds.intersects(enemyBounds))
                {
                    enemies.erase(enemies.begin() + k);
                    player.bullets.erase(player.bullets.begin() + i);
                    break;
                }
            }
        }

        // Спавн противников
        if (enemySpawnTimer < 20)
        {
            enemySpawnTimer++;
        }
        
        // Система волн врагов в стиле Touhou
        if (enemySpawnTimer >= 20 && enemies.size() < 10) // Ограничиваем количество врагов на экране
        {
            waveTimer++;
            float windowWidth = window.getSize().x;
            
            switch(currentWave % 5) { // 5 разных типов волн
                case 0: { // Линейная волна
                    float spacing = windowWidth / 6;
                    for(int i = 1; i <= 5; i++) {
                        spawnEnemy(spacing * i, 50.f, PATTERN_STRAIGHT, SHOOT_SPREAD);
                    }
                    break;
                }
                case 1: { // Спиральная волна
                    for(int i = 0; i < 4; i++) {
                        float angle = i * 90.0f;
                        float x = windowWidth/2 + cos(angle * 3.14159f/180.0f) * 100.f;
                        float y = 300 + sin(angle * 3.14159f/180.0f) * 100.f;
                        spawnEnemy(x, y, PATTERN_SPIRAL, SHOOT_SPIRAL);
                    }
                    break;
                }
                case 2: { // Волновая атака
                    for(int i = 0; i < 3; i++) {
                        spawnEnemy(windowWidth * 0.25f + i * windowWidth * 0.25f, 50.f, 
                                 PATTERN_WAVE, SHOOT_CIRCLE);
                    }
                    break;
                }
                case 3: { // Охотники
                    float randomX = rand() % int(windowWidth - 32);
                    spawnEnemy(randomX, 50.f, PATTERN_SINE, SHOOT_AIMED);
                    break;
                }
                case 4: { // Смешанная волна
                    spawnEnemy(windowWidth * 0.2f, 50.f, PATTERN_ZIGZAG, SHOOT_SPREAD);
                    spawnEnemy(windowWidth * 0.8f, 50.f, PATTERN_ZIGZAG, SHOOT_SPREAD);
                    spawnEnemy(windowWidth * 0.5f, 50.f, PATTERN_STRAIGHT, SHOOT_CIRCLE);
                    break;
                }
            }
            
            currentWave++;
            enemySpawnTimer = 0;
            waveTimer = 0;
        }

        // Движение и обработка противников
        for (size_t i = 0; i < enemies.size(); i++)
        {
            // Проверка коллизии противника с игроком
            sf::FloatRect enemyBounds(enemies[i].get_left(), enemies[i].get_top(),
                                      enemies[i].get_width(), enemies[i].get_height());
            sf::FloatRect playerBounds(player.get_left(), player.get_top(),
                                       player.get_width(), player.get_height());

            if (enemyBounds.intersects(playerBounds))
            {
                enemies.erase(enemies.begin() + i);
                player.HP--;
                break;
            }

            // Проверка коллизии пуль врага с игроком
            for (size_t j = 0; j < enemies[i].bullets.size(); j++)
            {
                sf::FloatRect bulletBounds = enemies[i].bullets[j].shape.getGlobalBounds();
                if (bulletBounds.intersects(playerBounds))
                {
                    enemies[i].bullets.erase(enemies[i].bullets.begin() + j);
                    player.HP--;
                    break;
                }
            }

            // Удаление противника за экраном
            if (enemies[i].get_top() > window.getSize().y)
            {
                enemies.erase(enemies.begin() + i);
                break;
            }
        }

        // Отрисовка
        window.clear();

        // Отрисовка игрока и его пуль
        player.draw(window);
        for (auto &bullet : player.bullets)
        {
            window.draw(bullet.shape);
        }

        // Отрисовка врагов и их пуль
        for (auto &enemy : enemies)
        {
            enemy.draw(window);
        }

        window.display();
    }

    return 0;
}
