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
    // Обновляем позицию
    Vector2f newPos = sprite.get_center();
    if (moveStates[DIR_UP]) newPos.y -= move_velocity;
    if (moveStates[DIR_DOWN]) newPos.y += move_velocity;
    if (moveStates[DIR_LEFT]) newPos.x -= move_velocity;
    if (moveStates[DIR_RIGHT]) newPos.x += move_velocity;
    
    // Ограничиваем движение в пределах окна
    float halfWidth = get_width() / 2.0f;
    float halfHeight = get_height() / 2.0f;
    newPos.x = std::max(halfWidth, std::min<float>(newPos.x, 800.0f - halfWidth));
    newPos.y = std::max(halfHeight, std::min<float>(newPos.y, 600.0f - halfHeight));
    
    set_center(newPos.x, newPos.y);
    
    // Обновляем пули и анимацию
    updateBullets();
    sprite.tick();
    
    check_animation_state();
}

void Player::check_animation_state()
{
    AnimationState newState = STATE_IDLE;
    
    if (moveStates[DIR_UP] || moveStates[DIR_DOWN]) {
        newState = STATE_FORWARD;
    }
    if (moveStates[DIR_LEFT]) {
        newState = STATE_LEFT;
    }
    if (moveStates[DIR_RIGHT]) {
        newState = STATE_RIGHT;
    }
    
    if (newState != currentState)
    {
        currentState = newState;
        
        switch (currentState)
        {
            case STATE_IDLE:
                sprite.init(BOUND_IDLE, SPRITE_DATA_IDLE.first, SPRITE_DATA_IDLE.second, SPRITE_FRAME_RATE);
                break;
            case STATE_FORWARD:
                sprite.init(BOUND_FORWARD, SPRITE_DATA_FORWARD.first, SPRITE_DATA_FORWARD.second, SPRITE_FRAME_RATE);
                break;
            case STATE_LEFT:
                sprite.init(BOUND_LEFT, SPRITE_DATA_LEFT.first, SPRITE_DATA_LEFT.second, SPRITE_FRAME_RATE);
                break;
            case STATE_RIGHT:
                sprite.init(BOUND_RIGHT, SPRITE_DATA_RIGHT.first, SPRITE_DATA_RIGHT.second, SPRITE_FRAME_RATE);
                break;
        }
    }
    
    // Обновляем масштаб в зависимости от текущего состояния
    float scaleX = (currentState == STATE_LEFT) ? -1.0f : 1.0f;
    sprite.setScale(scaleX, 1.0f);
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
