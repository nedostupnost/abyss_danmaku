#pragma once
#include <SFML/Graphics.hpp>
#include "../libprojectabyss/Player.h"
#include "../libprojectabyss/Enemy.h"
#include "../libprojectabyss/Bullet.h"

class Game {
protected:
    sf::RenderWindow window;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> enemyBullets;  // Новый вектор для пуль врагов
    sf::Font font;
    sf::Texture playerTexture, enemyTexture, bulletTexture, hitboxTexture;
    sf::Sprite hitboxSprite;
    int enemySpawnTimer;

    void loadResources();
    void processEvents();
    void updatePlayer();
    void updateEnemies();
    void updateBullets();
    void checkCollisions();
    void render();
    void spawnEnemies();

public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    void run();
};