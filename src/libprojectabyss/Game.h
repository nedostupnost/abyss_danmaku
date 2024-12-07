#pragma once
#include <SFML/Graphics.hpp>
#include "../libprojectabyss/Player.h"
#include "../libprojectabyss/Enemy.h"
#include "../libprojectabyss/Bullet.h"
#include <queue>

// Структура для описания волны врагов
struct WaveDescription {
    int enemyCount;
    std::vector<EnemyMovementPattern> movePatterns;
    std::vector<BulletPattern> shootPatterns;
    float spawnInterval;
    float enemySpeed;
    int enemyHP;
};

class Game {
protected:
    sf::RenderWindow window;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> enemyBullets;
    sf::Font font;
    sf::Texture playerTexture, enemyTexture, bulletTexture, hitboxTexture, playerBulletTexture;
    sf::Sprite hitboxSprite;
    
    // Параметры спавна врагов
    int enemySpawnTimer;
    int currentWave;
    int enemiesSpawned;
    std::queue<WaveDescription> waveQueue;
    WaveDescription* currentWaveDesc;

    void loadResources();
    void processEvents();
    void updatePlayer();
    void updateEnemies();
    void updateBullets();
    void checkCollisions();
    void render();
    void spawnEnemies();
    
    // Новые методы для системы волн
    void initializeWaves();
    void updateWaveSystem();
    WaveDescription createWaveDescription(int enemyCount, 
                                        const std::vector<EnemyMovementPattern>& movePatterns,
                                        const std::vector<BulletPattern>& shootPatterns,
                                        float spawnInterval = 10.f,
                                        float enemySpeed = 3.f,
                                        int enemyHP = 1);

public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    ~Game(); // Добавляем деструктор
    void run();
};