#pragma once
#include "abyssGame.hpp"


class Player
{
    private:
    Texture *texture;
    Sprite shape;
    RectangleShape hitbox;

    int level, exp, nextLevelExp;

    int hp, hpMax;

    int damage, damageMax;

    int score;


    public:

    Player();
    virtual ~Player();
    void update();
    void Draw();
};