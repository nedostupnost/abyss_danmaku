#include "Enemy.h"

// Конструктор класса Enemy
Enemy::Enemy(Texture *texture, Vector2u windowSize)
{
    this->MaxHP = rand() % 3 + 1; // Генерация случайного максимального здоровья
    this->HP = this->MaxHP; // Инициализация текущего здоровья
    this->shape.setTexture(*texture); // Установка текстуры на спрайт
    this->shape.setPosition(rand() % int(windowSize.x - (this->shape.getGlobalBounds().width * 1.5)), 
                            this->shape.getGlobalBounds().height - 100); // Установка случайной позиции
}
