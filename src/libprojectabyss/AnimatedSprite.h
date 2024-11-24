#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedSprite.h"

class AnimatedSprite : public BoundedSprite {
private:
    sf::IntRect first_bound, curr_bound;
    int curr_sprite, num_sprites;
    int frame_counter, max_frames;

    void next_sprite();
public:    
    AnimatedSprite();
    void init(sf::IntRect orig,
              int curr_sprite, int num_sprites,
              int max_frames);

    void tick();
};
