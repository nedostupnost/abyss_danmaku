#include "BoundedSprite.h"

BoundedSprite::BoundedSprite() : texture(nullptr) {}

void BoundedSprite::init_texture(sf::Texture* texture) {
    this->texture = texture;
    s.setTexture(*texture);
}

void BoundedSprite::init(sf::IntRect bound) {
    s.setTextureRect(bound);
}

void BoundedSprite::set_center(double x, double y) {
    s.setPosition(x - s.getGlobalBounds().width / 2.0,
                 y - s.getGlobalBounds().height / 2.0);
}

void BoundedSprite::move(double x, double y) {
    s.move(x, y);
}

sf::Vector2f BoundedSprite::get_center() const {
    return sf::Vector2f(s.getPosition().x + s.getGlobalBounds().width / 2.0,
                       s.getPosition().y + s.getGlobalBounds().height / 2.0);
}

double BoundedSprite::get_left() const {
    return s.getPosition().x;
}

double BoundedSprite::get_right() const {
    return s.getPosition().x + s.getGlobalBounds().width;
}

double BoundedSprite::get_top() const {
    return s.getPosition().y;
}

double BoundedSprite::get_bottom() const {
    return s.getPosition().y + s.getGlobalBounds().height;
}

double BoundedSprite::get_width() const {
    return s.getGlobalBounds().width;
}

double BoundedSprite::get_height() const {
    return s.getGlobalBounds().height;
}

void BoundedSprite::draw(sf::RenderTarget& target) const {
    target.draw(s);
}