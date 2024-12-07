#include "Bullet.h"

Bullet::Bullet(Texture *texture, Vector2f pos, BulletType bulletType, bool playerBullet) {
    shape.setTexture(*texture);
    
    // Сначала устанавливаем флаг владельца пули
    isPlayerBullet = playerBullet;
    
    // Устанавливаем нужную часть текстуры в зависимости от типа пули и владельца
    if (isPlayerBullet) {
        speed = 10.f;  // Скорость пуль игрока
        shape.setTextureRect(IntRect(0, 0, 16, 16)); 
    } else {
        speed = 5.f;
        // Используем разные спрайты для разных паттернов выстрелов противников
        switch (type) {
            default:
                break;
        }
    }
    
    shape.setPosition(pos);
    direction = Vector2f(0.f, isPlayerBullet ? -1.f : 1.f);  // Вверх для игрока, вниз для врагов
    wavePattern = false;
    waveTime = 0.f;
    type = bulletType;
}

void Bullet::update() {
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
