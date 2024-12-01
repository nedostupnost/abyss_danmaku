/**
 * @file AnimatedSprite.cpp
 * @brief Реализация анимированного спрайта
 */
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() : 
    curr_sprite(0), 
    num_sprites(0), 
    frame_counter(0), 
    max_frames(0),
    animation_mode(AnimationMode::LOOP),
    animation_finished(false),
    reverse_animation(false)
{}

void AnimatedSprite::init(
    sf::IntRect orig,
    int curr_sprite, 
    int num_sprites,
    int max_frames,
    AnimationMode mode
) {
    first_bound = orig;
    curr_bound = orig;
    this->curr_sprite = curr_sprite;
    this->num_sprites = num_sprites;
    this->max_frames = max_frames;
    this->animation_mode = mode;
    
    // Установка начального прямоугольника текстуры
    setTextureRect(sf::IntRect(
        first_bound.left + curr_sprite * first_bound.width,
        first_bound.top,
        first_bound.width,
        first_bound.height
    ));
}

void AnimatedSprite::next_sprite() {
    switch (animation_mode) {
        case AnimationMode::LOOP:
            curr_sprite = (curr_sprite + 1) % num_sprites;
            break;
        
        case AnimationMode::ONCE:
            if (curr_sprite < num_sprites - 1) {
                curr_sprite++;
            } else {
                animation_finished = true;
            }
            break;
        
        case AnimationMode::REVERSE_LOOP:
            if (!reverse_animation) {
                curr_sprite++;
                if (curr_sprite >= num_sprites - 1) {
                    reverse_animation = true;
                }
            } else {
                curr_sprite--;
                if (curr_sprite <= 0) {
                    reverse_animation = false;
                }
            }
            break;
    }

    // Обновление прямоугольника текстуры
    setTextureRect(sf::IntRect(
        first_bound.left + curr_sprite * first_bound.width,
        first_bound.top,
        first_bound.width,
        first_bound.height
    ));
}

void AnimatedSprite::tick() {
    if (animation_finished) return;

    frame_counter++;
    if (frame_counter >= max_frames) {
        next_sprite();
        frame_counter = 0;
    }
}

bool AnimatedSprite::isAnimationFinished() const {
    return animation_finished;
}

void AnimatedSprite::resetAnimation() {
    curr_sprite = 0;
    frame_counter = 0;
    animation_finished = false;
    reverse_animation = false;

    // Сброс прямоугольника текстуры
    setTextureRect(sf::IntRect(
        first_bound.left,
        first_bound.top,
        first_bound.width,
        first_bound.height
    ));
}