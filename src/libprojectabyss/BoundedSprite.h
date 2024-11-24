#pragma once
#include <SFML/Graphics.hpp>

class BoundedSprite {
protected:
    sf::Sprite s;
    sf::Texture* texture;

public:
    BoundedSprite();
    void init_texture(sf::Texture* texture);
    void init(sf::IntRect bound);
    void set_center(double x, double y);
    void move(double x, double y);
    sf::Vector2f get_center();
    
    double get_left();
    double get_right();
    double get_top();
    double get_bottom();
    double get_width();
    double get_height();
    
    void draw(sf::RenderTarget& target);
};
