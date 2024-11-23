#pragma once
#include "Bullet.h"

using namespace sf;

// Класс Player представляет игрока в игре
class Player
{
public:
    Sprite shape; // Спрайт игрока
    Texture* texture; // Указатель на текстуру для повторного использования
    int HP; // Текущее количество жизней
    int HPMax; // Максимальное количество жизней
    int shootTimer; // скорострельность
    std::vector<Bullet> bullets; // Вектор пуль игрока

    // Конструктор, принимает указатель на текстуру и размер окна
    Player(Texture *texture, Vector2u windowSize);
    ~Player() {} // Деструктор
};
