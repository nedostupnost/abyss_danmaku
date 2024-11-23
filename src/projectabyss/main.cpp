#include "../libprojectabyss/aLibs.h"  // Включаем заголовочный файл класса Bullet

using namespace sf;

int main(void)
{
    srand(static_cast<unsigned int>(time(NULL))); // Инициализация генератора случайных чисел
    RenderWindow window(VideoMode(800, 600), "Abyss Danmaku", Style::Default);
    window.setFramerateLimit(60);



    // говно старт *******************************************************************************

    Font font;
    font.loadFromFile("assets/arial.ttf");

    Texture playerTexture;
    playerTexture.loadFromFile("assets/image/player.png");

    Texture enemyTexture;
    enemyTexture.loadFromFile("assets/image/enemy.png");

    Texture bulletTexture;
    bulletTexture.loadFromFile("assets/image/bullet.png");

    // говно енд *******************************************************************************



    // ну херня для инициализации старт *******************************************************************************

    Player player(&playerTexture, window.getSize()); // Инициализация игрока
    int enemySpawnTimer = 0; // Таймер для спавна противников
    std::vector<Enemy> enemies; // Вектор для хранения противников

    // ну херня для инициализации енд *******************************************************************************




    while (window.isOpen()) // Основной цикл игры 
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close(); // Закрытие окна
        }



        // Управление игроком
        if (Keyboard::isKeyPressed(Keyboard::Numpad8)) { player.shape.move(0.f, -5.f); } // Вверх
        if (Keyboard::isKeyPressed(Keyboard::Numpad4)) { player.shape.move(-5.f, 0.f); } // Влево
        if (Keyboard::isKeyPressed(Keyboard::Numpad5)) { player.shape.move(0.f, 5.f); } // Вниз
        if (Keyboard::isKeyPressed(Keyboard::Numpad6)) { player.shape.move(5.f, 0.f); } // Вправо



        // Проверка границ окна
        if (player.shape.getPosition().x <= 0) { player.shape.setPosition(0.f, player.shape.getPosition().y); } // Левая граница
        if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) { player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y); } // Правая граница
        if (player.shape.getPosition().y <= 0) { player.shape.setPosition(player.shape.getPosition().x, 0.f); } // Верхняя граница
        if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) { player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height); } // Нижняя граница



        // Таймер стрельбы
        if (player.shootTimer < 10) { player.shootTimer++; }

        // Стрельба
        if (Keyboard::isKeyPressed(Keyboard::Z) && player.shootTimer >= 10)
        {
            player.bullets.push_back(Bullet(&bulletTexture, player.shape.getPosition())); // Создание новой пули
            player.shootTimer = 0; // Сброс таймера
        }



        // Движение и обработка пуль
        for (size_t i = 0; i < player.bullets.size(); i++)
        {
            player.bullets[i].shape.move(0.f, -10.f); // Перемещение пули вверх

            // Удаление пули за экраном
            if (player.bullets[i].shape.getPosition().y < 0)
            {
                player.bullets.erase(player.bullets.begin() + i); // Удалить пулю
                break;
            }

            // Проверка коллизии пуль с противниками
            for (size_t k = 0; k < enemies.size(); k++)
            {
                if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                {
                    enemies.erase(enemies.begin() + k); // Удаление противника
                    player.bullets.erase(player.bullets.begin() + i); // Удаление пули
                    break;
                }
            }
        }



        // Спавн противников
        if (enemySpawnTimer < 20) { enemySpawnTimer++; }
        if (enemySpawnTimer >= 20)
        {
            enemies.push_back(Enemy(&enemyTexture, window.getSize())); // Создание нового противника
            enemySpawnTimer = 0; // Сброс таймера
        }




        // Движение и обработка противников
        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].shape.move(0.f, 3.f); // Движение вниз

            // Удаление противника за экраном
            if (enemies[i].shape.getPosition().y > window.getSize().y)
            {
                enemies.erase(enemies.begin() + i); // Удалить противника
                break;
            }

            // Проверка коллизии противника с игроком
            if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
            {
                enemies.erase(enemies.begin() + i); // Удаление противника
                player.HP--; // Уменьшение здоровья игрока
                break;
            }
        }

        window.clear(); // Очистка окна


        

        // Отрисовка игрока, пуль и противников
        window.draw(player.shape);
        for (auto& bullet : player.bullets) { window.draw(bullet.shape); }
        for (auto& enemy : enemies) { window.draw(enemy.shape); }

        window.display(); // Вывод в окно
    }

    return 0; // Завершение программы
}
