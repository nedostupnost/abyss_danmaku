/**
 * @file GameState.h
 * @brief Определение состояния игры
 */
#pragma once
#include <memory>
#include "../entities/Player.h"
#include "../entities/Enemy.h"

struct GameState {
    enum class Status {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER,
        WIN
    };

    Status currentStatus = Status::MENU;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    int score = 0;
    int level = 1;
    
    // Методы управления состоянием игры
    void reset() {
        currentStatus = Status::MENU;
        score = 0;
        level = 1;
        player.reset();
        enemies.clear();
    }

    void startGame() {
        currentStatus = Status::PLAYING;
        // Инициализация игрока и врагов
        player = std::make_unique<Player>();
        // Загрузка первого уровня
    }

    void pauseGame() {
        if (currentStatus == Status::PLAYING) {
            currentStatus = Status::PAUSED;
        }
    }

    void resumeGame() {
        if (currentStatus == Status::PAUSED) {
            currentStatus = Status::PLAYING;
        }
    }
};