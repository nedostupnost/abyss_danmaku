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
    sf::Vector2f get_center() const;
    
    double get_left() const;
    double get_right() const;
    double get_top() const;
    double get_bottom() const;
    double get_width() const;
    double get_height() const;
    
    void draw(sf::RenderTarget& target) const;
};
