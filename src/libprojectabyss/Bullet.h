#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace sf;

// Типы пуль
enum BulletType {
    BULLET_SINGLE_TYPE = 0,   // Тип пули для одиночного выстрела
    BULLET_CIRCLE_TYPE = 1,   // Тип пули для кругового выстрела
    BULLET_SPIRAL_TYPE = 2,   // Тип пули для спирального выстрела
    BULLET_FAN_TYPE = 3,      // Тип пули для веерного выстрела
    BULLET_WAVE_TYPE = 4,     // Тип пули для волнового выстрела
    BULLET_AIMED_TYPE = 5     // Тип пули для прицельного выстрела
};

// Класс Bullet представляет пулю в игре
class Bullet
{
public:
    Sprite shape;           // Спрайт пули
    Vector2f direction;     // Направление движения
    bool wavePattern;       // Флаг волнового движения
    float waveTime;         // Время для волнового движения
    float speed;           // Скорость пули
    BulletType type;       // Тип пули
    bool isPlayerBullet;  // Добавляем флаг для определения, чья пуля

    // Конструктор, принимает указатель на текстуру, позицию и тип пули
    Bullet(Texture *texture, Vector2f pos, BulletType bulletType = BULLET_SINGLE_TYPE, bool playerBullet = false);
    
    // Обновление положения пули
    void update();
};
