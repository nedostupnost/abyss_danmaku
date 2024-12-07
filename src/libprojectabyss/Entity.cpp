#include "Entity.h"

Entity::Entity(Texture* texture, Vector2f position)
    : HP(1),
      HPMax(1),
      shootTimer(0)
{
    sprite.init_texture(texture);
    sprite.set_center(position.x, position.y);
}

void Entity::draw(RenderTarget& target) {
    sprite.draw(target);
    for (auto& bullet : bullets) {
        target.draw(bullet.shape);
    }
}

void Entity::updateBullets() {
    for (size_t i = 0; i < bullets.size();) {
        bullets[i].update();
        
        Vector2f pos = bullets[i].shape.getPosition();
        if (pos.y > 600 || pos.y < 0 || pos.x < 0 || pos.x > 800) {
            bullets.erase(bullets.begin() + i);
        } else {
            ++i;
        }
    }
}

void Entity::takeDamage(int damage) {
    HP -= damage;
    if (HP < 0) HP = 0;
}

bool Entity::isAlive() const {
    return HP > 0;
}

void Entity::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

void Entity::set_center(float x, float y) {
    sprite.set_center(x, y);
}
