#pragma once
#include "Bullet.h"

using namespace sf;

// Класс Enemy представляет противника в игре
class Enemy
{
public:
    Sprite shape; // Спрайт противника
    int HP; // Текущее здоровье
    int MaxHP; // Максимальное здоровье

    // Конструктор, принимает указатель на текстуру и размер окна
    Enemy(Texture *texture, Vector2u windowSize);
    ~Enemy(){}; // Деструктор
};
