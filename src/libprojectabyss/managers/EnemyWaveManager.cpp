/**
 * @file EnemyWaveManager.cpp
 * @brief Реализация менеджера волн противников
 */
#include "EnemyWaveManager.h"

EnemyWaveManager::EnemyWaveManager(
    ResourceManager& resourceMgr, 
    sf::RenderWindow& gameWindow
) : 
    resourceManager(resourceMgr),
    window(gameWindow),
    randomGenerator(std::random_device{}()),
    currentLevel(1),
    spawnTimer(0.0f)
{
    // Генерация первой волны при инициализации
    generateWave(calculateWaveDifficulty());
}

void EnemyWaveManager::update(float deltaTime) {
    if (!currentWave) return;

    // Обновление таймера спауна
    spawnTimer += deltaTime;

    // Спаун противников с интервалом
    if (spawnTimer >= currentWave->spawnInterval) {
        spawnEnemy();
        spawnTimer = 0.0f;
    }

    // Обновление состояния каждого противника
    for (auto& enemy : currentWave->enemies) {
        enemy->update(deltaTime);
    }

    // Удаление мертвых противников
    currentWave->enemies.erase(
        std::remove_if(currentWave->enemies.begin(), currentWave->enemies.end(),
            [](const auto& enemy) { return enemy->isDead(); }),
        currentWave->enemies.end()
    );

    // Если все противники уничтожены, генерируем новую волну
    if (currentWave->enemies.empty()) {
        currentLevel++;
        generateWave(calculateWaveDifficulty());
    }
}

void EnemyWaveManager::render(sf::RenderWindow& window) {
    if (!currentWave) return;

    // Отрисовка каждого живого противника
    for (auto& enemy : currentWave->enemies) {
        enemy->draw(window);
    }
}

int EnemyWaveManager::getAliveEnemyCount() const {
    return currentWave ? currentWave->enemies.size() : 0;
}

void EnemyWaveManager::generateWave(WaveDifficulty difficulty) {
    // Создание новой волны
    currentWave = std::make_unique<EnemyWave>();
    currentWave->difficulty = difficulty;

    // Определение параметров волны в зависимости от сложности
    switch (difficulty) {
        case WaveDifficulty::EASY:
            currentWave->totalEnemies = 5 + currentLevel;
            currentWave->spawnInterval = 2.0f;
            break;
        case WaveDifficulty::MEDIUM:
            currentWave->totalEnemies = 10 + currentLevel * 2;
            currentWave->spawnInterval = 1.5f;
            break;
        case WaveDifficulty::HARD:
            currentWave->totalEnemies = 15 + currentLevel * 3;
            currentWave->spawnInterval = 1.0f;
            break;
        case WaveDifficulty::EXTREME:
            currentWave->totalEnemies = 20 + currentLevel * 4;
            currentWave->spawnInterval = 0.5f;
            break;
    }
}

void EnemyWaveManager::spawnEnemy() {
    if (!currentWave || currentWave->enemies.size() >= currentWave->totalEnemies) 
        return;

    // Случайная позиция по горизонтали
    std::uniform_real_distribution<float> xDistribution(0, window.getSize().x);
    float randomX = xDistribution(randomGenerator);

    // Создание противника
    auto enemyTexture = resourceManager.getTexture("enemy");
    auto enemy = std::make_unique<Enemy>(
        &enemyTexture, 
        sf::Vector2f(randomX, 0), 
        selectEnemyType()
    );

    currentWave->enemies.push_back(std::move(enemy));
}

EnemyMovementPattern EnemyWaveManager::selectEnemyType() {
    // Случайный выбор паттерна движения
    std::uniform_int_distribution<int> patternDistribution(0, 3);
    return static_cast<EnemyMovementPattern>(patternDistribution(randomGenerator));
}

WaveDifficulty EnemyWaveManager::calculateWaveDifficulty() {
    // Увеличение сложности с ростом уровня
    if (currentLevel < 3) return WaveDifficulty::EASY;
    if (currentLevel < 6) return WaveDifficulty::MEDIUM;
    if (currentLevel < 10) return WaveDifficulty::HARD;
    return WaveDifficulty::EXTREME;
}