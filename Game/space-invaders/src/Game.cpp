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

    // Load shaders
    std::shared_ptr<Shader> spriteShader = ResourceManager::LoadShader("res/shaders/Sprite.vertex", "res/shaders/Sprite.frag", "Test");

    spriteShader->Bind();
    glm::mat4 ortho = glm::ortho(0.f, WIDTH, HEIGHT, 0.f, -1.f, 1.f);
    spriteShader->SetUniformMat4f("u_Projection", ortho);
    spriteShader->SetUniform1i("u_Image", 0);
    
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);
    
    // Create Player
    m_PlayerManager = std::make_unique<PlayerManager>(*this);
    m_PlayerManager->CreatePlayer(WIDTH, HEIGHT);
}

void Game::Update(float deltaTime)
{
    // move player projectiles
    for (auto& projectile : m_PlayerProjectiles)
    {
        // check boundaries
        if (projectile.Position.y <= 0)
        {
            projectile.Destroyed = true;
            continue;
        }
        
        projectile.Position.y -= projectile.Velocity.y * deltaTime;
    }
}

void Game::ProcessInput(float deltaTime, const Input& input)
{
    m_PlayerManager->ProcessInput(deltaTime, input, WIDTH);
}

void Game::Render()
{
    m_PlayerManager->Render(*m_SpriteRenderer);

    RenderProjectiles();
    RemoveDestroyedProjectiles();
}

void Game::Close()
{
    ResourceManager::ClearAll();
}

void Game::RenderProjectiles() const
{
    for (auto& projectile : m_PlayerProjectiles)
    {
        projectile.Draw(*m_SpriteRenderer);
    }
}

void Game::AddEnemyProjectile(GameObject&& projectile)
{
    GameObject newProjectile = projectile;
    m_EnemyProjectiles.emplace_back(newProjectile);
}

void Game::AddPlayerProjectile(GameObject&& projectile)
{
    GameObject newProjectile = projectile;
    m_PlayerProjectiles.emplace_back(newProjectile);
}

void Game::RemoveDestroyedProjectiles()
{
    auto playerIterator = m_PlayerProjectiles.begin();
    while (playerIterator != m_PlayerProjectiles.end())
    {
        if (playerIterator->Destroyed)
        {
            playerIterator = m_PlayerProjectiles.erase(playerIterator);
            std::cout << "Player projectile erased!\n";
        }
        else
        {
            ++playerIterator;
        }
    }

    auto enemyIterator = m_EnemyProjectiles.begin();
    while (enemyIterator != m_EnemyProjectiles.end())
    {
        if (enemyIterator->Destroyed)
        {
            enemyIterator = m_EnemyProjectiles.erase(enemyIterator);
            std::cout << "Enemy projectile erased!\n";
        }
        else
        {
            ++enemyIterator;
        }
    }
}
