#include "Player.h"

// Векторы движения для каждого направления
const float MOVE_VECTOR[4][2] = {
    {0.0f, -1.0f},  // UP
    {1.0f, 0.0f},   // RIGHT
    {0.0f, 1.0f},   // DOWN
    {-1.0f, 0.0f}   // LEFT
};

Player::Player(Texture* texture, Vector2u windowSize)
    : Entity(texture, Vector2f(windowSize.x / 2.0f, windowSize.y * 0.8f))
{
    HP = 5;
    HPMax = 5;
    
    // Инициализация анимированного спрайта
    sprite.init_texture(texture);
    sprite.init(BOUND_IDLE, SPRITE_DATA_IDLE.first, SPRITE_DATA_IDLE.second, SPRITE_FRAME_RATE);
    sprite.setScale(1.0f, 1.0f);
}

void Player::update()
{
    // Обработка движения
    float totalX = 0.0f;
    float totalY = 0.0f;
    
    for (int i = 0; i < 4; ++i) {
        if (moveStates[i]) {
            totalX += MOVE_VECTOR[i][0] * move_velocity;
            totalY += MOVE_VECTOR[i][1] * move_velocity;
        }
    }
    
    // Применяем движение с учетом границ экрана
    Vector2f newPos = get_center();
    newPos.x += totalX;
    newPos.y += totalY;
    
    // Проверка границ экрана
    float halfWidth = get_width() / 2.0f;
    float halfHeight = get_height() / 2.0f;
    
    // Ограничиваем позицию в пределах экрана
    newPos.x = std::max(halfWidth, std::min(newPos.x, 800.0f - halfWidth));
    newPos.y = std::max(halfHeight, std::min(newPos.y, 600.0f - halfHeight));
    
    // Устанавливаем новую позицию
    set_center(newPos.x, newPos.y);
    
    // Обновляем пули и анимацию
    updateBullets();
    sprite.tick();
    
    // Сохраняем текущий масштаб
    float scaleX = (currentState == STATE_LEFT) ? -1.0f : 1.0f;
    sprite.setScale(scaleX, 1.0f);
    
    check_animation_state();
}

void Player::check_animation_state()
{
    AnimationState newState = STATE_IDLE;
    
    if (moveStates[DIR_LEFT]) {
        newState = STATE_LEFT;
    }
    else if (moveStates[DIR_RIGHT]) {
        newState = STATE_RIGHT;
    }
    else if (moveStates[DIR_UP] || moveStates[DIR_DOWN]) {
        newState = STATE_FORWARD;
    }
    
    if (newState != currentState) {
        currentState = newState;
        switch (currentState) {
            case STATE_IDLE:
                sprite.init(BOUND_IDLE, SPRITE_DATA_IDLE.first, SPRITE_DATA_IDLE.second, SPRITE_FRAME_RATE);
                break;
            case STATE_FORWARD:
                sprite.init(BOUND_FORWARD, SPRITE_DATA_FORWARD.first, SPRITE_DATA_FORWARD.second, SPRITE_FRAME_RATE);
                break;
            case STATE_LEFT:
            case STATE_RIGHT:
                sprite.init(BOUND_SIDE, SPRITE_DATA_SIDE.first, SPRITE_DATA_SIDE.second, SPRITE_FRAME_RATE);
                break;
        }
        // Восстанавливаем масштаб после инициализации спрайта
        float scaleX = (currentState == STATE_LEFT) ? -1.0f : 1.0f;
        sprite.setScale(scaleX, 1.0f);
    }
}

void Player::set_move_state(int direction, bool state)
{
    if (direction >= 0 && direction < 4) {
        moveStates[direction] = state;
    }
}

void Player::shoot(Texture* bulletTexture)
{
    Vector2f pos = get_center();
    bullets.emplace_back(bulletTexture, pos, BULLET_SINGLE_TYPE, true);
}
