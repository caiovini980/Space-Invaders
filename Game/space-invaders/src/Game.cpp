#include "Game.h"

#include <iostream>
#include <memory>

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
    
    ResourceManager::LoadTexture("res/textures/awesomeface.png", "Face", true);
    
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);
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

    m_SpriteRenderer->Draw(*ResourceManager::GetTexture("Face"), {200.f, 200.f}, {100.f, 100.f});
}

void Game::Close()
{
    std::cout << "Closing game...\n";

    ResourceManager::ClearAll();
}
