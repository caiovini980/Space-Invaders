#include "Game.h"

#include <iostream>
#include <memory>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "PlayerManager.h"
#include "glm/ext/matrix_clip_space.hpp"

Game::Game() {}

Game::~Game() {}

void Game::Init()
{
    std::cout << "Game starting...\n";

    // Load shaders
    std::shared_ptr<Shader> spriteShader = ResourceManager::LoadShader("res/shaders/Sprite.vertex", "res/shaders/Sprite.frag", "Test");

    spriteShader->Bind();
    glm::mat4 ortho = glm::ortho(0.f, WIDTH, HEIGHT, 0.f, -1.f, 1.f);
    spriteShader->SetUniformMat4f("u_Projection", ortho);
    spriteShader->SetUniform1i("u_Image", 0);
    
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);
    
    // Create Player
    m_PlayerManager = std::make_unique<PlayerManager>();
    m_PlayerManager->CreatePlayer(WIDTH, HEIGHT);
}

void Game::Update(float deltaTime)
{
    // std::cout << "Game running\n";
}

void Game::ProcessInput(float deltaTime, const Input& input)
{
    // std::cout << "Getting input\n";
    m_PlayerManager->ProcessInput(deltaTime, input);
}

void Game::Render()
{
    // Render Player
    m_PlayerManager->Render(*m_SpriteRenderer);
}

void Game::Close()
{
    std::cout << "Closing game...\n";

    ResourceManager::ClearAll();
}
