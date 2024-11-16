#pragma once

#include "abyssBullet.hpp"


class Player
{
    private:
    Texture *texture;
    Sprite sprite;
    RectangleShape hitbox;

    int level, exp, nextLevelExp;

    int hp, hpMax;

    int damage, damageMax;

    int score;



    public:

    Player(Texture *texture);
    virtual ~Player();

    void Movement();
    void Update();
    void Draw(RenderTarget &target);
};