#include "Player.h"

// Конструктор класса Player
Player::Player(Texture *texture, Vector2u windowSize)
{
    this->HPMax = 10; // Максимальные жизни
    this->HP = this->HPMax; // Инициализация текущих жизней
    this->texture = texture; // Сохранение текстуры
    this->shape.setTexture(*texture); // Установка текстуры на спрайт
    this->shape.setPosition((windowSize.x - this->shape.getGlobalBounds().width) / 2, 
                            windowSize.y - this->shape.getGlobalBounds().height); // Центрирование позиции
    this->shape.setScale(0.85f, 0.85f); // Масштабирование спрайта
}
