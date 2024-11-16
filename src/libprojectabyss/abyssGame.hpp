#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;


class Game
{
    private:
    RenderWindow *window;
    
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