#include "Game.h"

#include <iostream>
#include <memory>

#include "GameLevel.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "glm/ext/matrix_clip_space.hpp"

Game::Game() {}

Game::~Game() {}

void Game::Init()
{
    std::cout << "Game starting...\n";

    std::shared_ptr<Shader> spriteShader = ResourceManager::LoadShader("res/shaders/Sprite.vertex", "res/shaders/Sprite.frag", "Test");

    spriteShader->Bind();
    glm::mat4 ortho = glm::ortho(0.f, 800.f, 600.f, 0.f, -1.f, 1.f);
    spriteShader->SetUniformMat4f("u_Projection", ortho);
    spriteShader->SetUniform1i("u_Image", 0);
    
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

    m_Level = std::make_unique<GameLevel>(800, 600);
}

void Game::Update(float deltaTime)
{
    std::cout << "Game running\n";

    m_Level->Update(deltaTime);
}

void Game::ProcessInput(float deltaTime)
{
    std::cout << "Getting input\n";
}

void Game::Render()
{
    m_Level->Render(*m_SpriteRenderer);
}

void Game::Close()
{
    std::cout << "Closing game...\n";

    ResourceManager::ClearAll();
}
