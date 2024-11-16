
#include "abyssGame.hpp"

Game::Game(RenderWindow *window)
{
    this->window = window;
    this->window->setFramerateLimit(60);

    playerTexture.loadFromFile("assets/image/reimu.png");

    player = new Player(&this->playerTexture);


}
Game::~Game()
{
    delete player;
}

void Game::Update()
{
    player->Update();
}

void Game::Draw()
{
    window->clear();
    
    player->Draw(*window);

    window->display();
}