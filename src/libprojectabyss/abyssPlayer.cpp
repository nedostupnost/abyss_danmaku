#include "abyssPlayer.hpp"

Player::Player(Texture *texture) : level(1), exp(0), nextLevelExp(100), hp(3), hpMax(5), damage(1), damageMax(3), score(0)
{
    this->texture = texture;
    this->sprite.setTexture(*this->texture);

}

Player::~Player()
{

}

void Player::Movement()
{
    if(Keyboard::isKeyPressed(Keyboard::Key(0)))
    {
        std::cout << "A" << std::endl;
    }
}

void Player::Update()
{
    this->Movement();
}

void Player::Draw(RenderTarget &target)
{
    target.draw(this->sprite);
}