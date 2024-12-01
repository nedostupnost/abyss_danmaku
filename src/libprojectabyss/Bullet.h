#pragma once
#include <SFML/Graphics.hpp>
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
    BULLET_TYPE_1 = 0,  // Первый тип пули (0-15 пикселей по x)
    BULLET_TYPE_2 = 1,  // Второй тип пули (16-31 пикселей по x)
    BULLET_TYPE_3 = 2,  // Третий тип пули (32-47 пикселей по x)
    BULLET_TYPE_4 = 3,  // Четвертый тип пули (48-63 пикселей по x)
    BULLET_TYPE_5 = 4   // Пятый тип пули (64-79 пикселей по x)
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

    // Конструктор, принимает указатель на текстуру, позицию и тип пули
    Bullet(Texture *texture, Vector2f pos, BulletType bulletType = BULLET_TYPE_1) {
        shape.setTexture(*texture);
        
        // Устанавливаем нужную часть текстуры в зависимости от типа пули
        shape.setTextureRect(IntRect(bulletType * 16, 0, 16, 16));
        
        shape.setPosition(pos);
        direction = Vector2f(0.f, 1.f);  // По умолчанию движение вниз
        wavePattern = false;
        waveTime = 0.f;
        speed = 5.f;
        type = bulletType;
    }
    
    void update() {
        if (wavePattern) {
            // Для волнового паттерна добавляем боковое смещение
            waveTime += 0.1f;
            float xOffset = std::sin(waveTime) * 2.f;
            shape.move(xOffset, speed);
        } else {
            // Для обычного движения используем направление
            shape.move(direction * speed);
        }
    }

    ~Bullet(){};
};
