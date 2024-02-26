#include "Game.h"

#include <iostream>

Game::Game() {}

Game::~Game() {}

void Game::Init()
{
    std::cout << "Game starting...\n";
}

void Game::Update(float deltaTime)
{
    std::cout << "Game running\n";
}

void Game::ProcessInput(float deltaTime)
{
    std::cout << "Getting input\n";
}

void Game::Render()
{
    std::cout << "Game rendering\n";
}

void Game::Close()
{
    std::cout << "Closing game...\n";
}
