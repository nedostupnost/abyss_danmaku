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
    std::vector<Enemy> enemies;

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
            enemy.shoot(&bulletTexture); // Враги теперь стреляют
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
        if (enemySpawnTimer >= 20)
        {
            float randomX = rand() % int(window.getSize().x - 32);
            // EnemyMovementPattern pattern = static_cast<EnemyMovementPattern>(3);
            EnemyMovementPattern pattern = PATTERN_CIRCLE;
            enemies.push_back(Enemy(&enemyTexture, Vector2f(randomX, 50.f), pattern));
            enemySpawnTimer = 0;
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