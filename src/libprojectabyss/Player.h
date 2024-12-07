#pragma once
#include "Entity.h"
#include <array>

// Константы для анимации
const sf::IntRect BOUND_IDLE(32 * 4, 0, 32, 48);
const std::pair<int, int> SPRITE_DATA_IDLE(0, 4);
const sf::IntRect BOUND_FORWARD(0, 0, 32, 48);
const std::pair<int, int> SPRITE_DATA_FORWARD(0, 4);
const sf::IntRect BOUND_LEFT(32 * 8, 0, 32, 48);  // Второй столбец для движения влево
const std::pair<int, int> SPRITE_DATA_LEFT(0, 4);
const sf::IntRect BOUND_RIGHT(32 * 12, 0, 32, 48); // Третий столбец для движения вправо
const std::pair<int, int> SPRITE_DATA_RIGHT(0, 4);

const int SPRITE_FRAME_RATE = 6;

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

class Player : public Entity {
private:
    static constexpr float move_velocity = 5.0f;
    bool moveStates[4] = {false, false, false, false};
    AnimationState currentState = STATE_IDLE;

public:
    Player(Texture* texture, Vector2u windowSize);
    
    void update() override;
    void shoot(Texture* bulletTexture) override;
    void set_move_state(int direction, bool state);

private:
    void check_animation_state();
};