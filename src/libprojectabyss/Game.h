#pragma once
#include <SFML/Graphics.hpp>
#include "../libprojectabyss/Player.h"
#include "../libprojectabyss/Enemy.h"
#include "../libprojectabyss/Bullet.h"

class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Enemy> enemies;
    sf::Font font;
    sf::Texture playerTexture, enemyTexture, bulletTexture;
    int enemySpawnTimer;

    void loadResources();
    void processEvents();
    void updatePlayer();
    void updateEnemies();
    void checkCollisions();
    void render();
    void spawnEnemies();

public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    void run();
};