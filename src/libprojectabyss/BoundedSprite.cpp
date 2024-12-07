#include "BoundedSprite.h"

BoundedSprite::BoundedSprite() {
    texture = nullptr;
}

void BoundedSprite::init_texture(sf::Texture* texture) {
    this->texture = texture;
    s.setTexture(*texture);
}

void BoundedSprite::init(sf::IntRect bound) {
    s.setTextureRect(bound);
    s.setOrigin(bound.width / 2.0, bound.height / 2.0);
}

void BoundedSprite::set_center(double x, double y) {
    s.setPosition(x, y);
}

void BoundedSprite::move(double x, double y) {
    s.move(x, y);
}

sf::Vector2f BoundedSprite::get_center() {
    return s.getPosition();
}

double BoundedSprite::get_left() {
    return s.getPosition().x - s.getOrigin().x;
}

double BoundedSprite::get_right() {
    return get_left() + get_width();
}

double BoundedSprite::get_top() {
    return s.getPosition().y - s.getOrigin().y;
}

double BoundedSprite::get_bottom() {
    return get_top() + get_height();
}

double BoundedSprite::get_width() {
    return s.getTextureRect().width;
}

double BoundedSprite::get_height() {
    return s.getTextureRect().height;
}

void BoundedSprite::setScale(float scaleX, float scaleY) {
    s.setScale(scaleX, scaleY);
}

void BoundedSprite::draw(sf::RenderTarget& target) {
    target.draw(s);
}