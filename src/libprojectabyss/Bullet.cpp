#include "Bullet.h"

// Конструктор класса Bullet
Bullet::Bullet(Texture *texture, Vector2f pos)
{
    this->shape.setTexture(*texture); // Устанавливаем текстуру
    this->shape.setPosition(pos); // Устанавливаем позицию
}
