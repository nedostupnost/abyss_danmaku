#include "../libprojectabyss/abyssGame.hpp"
int main(void)
{
    RenderWindow window(VideoMode(800, 600), "projectabbys", Style::Default);

    window.setFramerateLimit(60);

    Game game(&window);

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }

        game.Update();
        game.Draw();
    }

    return 0;
}