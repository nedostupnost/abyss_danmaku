#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;

// Класс Bullet представляет пулю в игре
class Bullet
{
public:
    Sprite shape; // Спрайт пули

    // Конструктор, принимает указатель на текстуру и позицию
    Bullet(Texture *texture, Vector2f pos);
    ~Bullet(){}; // Деструктор
};
