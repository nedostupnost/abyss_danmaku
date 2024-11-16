#pragma once
#include "abyssPlayer.hpp"

class Game
{
    private:
    RenderWindow *window;

    Player *player;
    Texture playerTexture;
    
    public:
    Game(RenderWindow *window);
    virtual ~Game();

    inline RenderWindow& getWindow()
    {
        return *this->window;
    }

    void Update();
    void Draw();
};