#include "Player.h"

// Состояния анимации
enum AnimationState {
    STATE_IDLE,
    STATE_FORWARD,
    STATE_LEFT,
    STATE_RIGHT
};

// Направления движения
enum Direction {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

// Векторы движения для каждого направления
const float MOVE_VECTOR[4][2] = {
    {0.0f, -1.0f},  // UP
    {1.0f, 0.0f},   // RIGHT
    {0.0f, 1.0f},   // DOWN
    {-1.0f, 0.0f}   // LEFT
};

Player::Player(Texture* texture, Vector2u windowSize) {
    this->shootTimer = 10;
    this->HPMax = 3;
    this->HP = this->HPMax;
    
    // Инициализация анимированного спрайта
    sprite.init_texture(texture);
    sprite.init(BOUND_IDLE, SPRITE_DATA_IDLE.first, SPRITE_DATA_IDLE.second, SPRITE_FRAME_RATE);
    
    // Установка начальной позиции
    sprite.set_center(windowSize.x / 2.0f, windowSize.y - sprite.get_height());
    
    // Инициализация параметров движения
    animation_state = STATE_IDLE;
    move_velocity = 3.0f;
    for (int i = DIR_UP; i <= DIR_LEFT; i++) {
        movement_direction[i] = false;
    }
}

void Player::update() {
    // Обновление анимации
    sprite.tick();
    
    // Обновление позиции
    move();
    
    // Проверка и обновление состояния анимации
    check_animation_state();
}

void Player::move() {
    // Движение игрока
    for (int i = DIR_UP; i <= DIR_LEFT; i++) {
        if (movement_direction[i]) {
            sprite.move(MOVE_VECTOR[i][0] * move_velocity, 
                       MOVE_VECTOR[i][1] * move_velocity);
        }
    }
    
    // Проверка границ экрана
    const float sizeMultiplier = 0.15f;
    if (sprite.get_left() < -sprite.get_width() * sizeMultiplier) {
        sprite.set_center(sprite.get_width() / 2.0f - sprite.get_width() * sizeMultiplier,
                         sprite.get_center().y);
    }
    else if (sprite.get_right() > 800 + sprite.get_width() * sizeMultiplier) {
        sprite.set_center(800 - sprite.get_width() / 2.0f + sprite.get_width() * sizeMultiplier,
                         sprite.get_center().y);
    }
    if (sprite.get_top() < -sprite.get_height() * sizeMultiplier) {
        sprite.set_center(sprite.get_center().x,
                         sprite.get_height() / 2.0f - sprite.get_height() * sizeMultiplier);
    }
    else if (sprite.get_bottom() > 600 + sprite.get_height() * sizeMultiplier) {
        sprite.set_center(sprite.get_center().x,
                         600 - sprite.get_height() / 2.0f + sprite.get_height() * sizeMultiplier);
    }
}

void Player::check_animation_state() {
    int new_state = STATE_IDLE;
    
    if (movement_direction[DIR_LEFT]) {
        new_state = STATE_LEFT;
    }
    else if (movement_direction[DIR_RIGHT]) {
        new_state = STATE_RIGHT;
    }
    else if (movement_direction[DIR_UP] || movement_direction[DIR_DOWN]) {
        new_state = STATE_FORWARD;
    }
    
    if (new_state != animation_state) {
        animation_state = new_state;
        switch (animation_state) {
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
}

void Player::set_move_state(int dir, bool state) {
    if (dir >= DIR_UP && dir <= DIR_LEFT) {
        movement_direction[dir] = state;
    }
}

void Player::draw(sf::RenderTarget& target) {
    sprite.draw(target);
}