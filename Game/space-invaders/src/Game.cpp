#include "Game.h"

#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "players/PlayerManager.h"
#include "Collision.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "ui/UIManager.h"

Game::Game() {}

Game::~Game() {}

void Game::Init()
{
    std::cout << "Game starting...\n";

    // Load shaders
    std::shared_ptr<Shader> spriteShader = ResourceManager::LoadShader("res/shaders/Sprite.vertex",
        "res/shaders/Sprite.frag", "Test");

    spriteShader->Bind();
    glm::mat4 ortho = glm::ortho(0.f, WIDTH, HEIGHT, 0.f, -1.f, 1.f);
    spriteShader->SetUniformMat4f("u_Projection", ortho);
    spriteShader->SetUniform1i("u_Image", 0);
    
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);
    
    // Create Player
    m_PlayerManager = std::make_unique<PlayerManager>(*this);
    m_PlayerManager->CreatePlayer(WIDTH, HEIGHT);

    m_Level = std::make_unique<GameLevel>(WIDTH, HEIGHT, *this);

    m_UIManager = std::make_unique<UIManager>(WIDTH, HEIGHT);
}

void Game::Update(float deltaTime)
{
    if(m_CurrentState == EGameState::Playing)
    {
        if(m_Level->IsEveryEnemyKilled())
        {
            HandleGameWon();
            return;
        }

        UpdatePlayerProjectiles(deltaTime);
        UpdateEnemyProjectiles(deltaTime);
        
        m_Level->Update(deltaTime);
    }
    else if(m_CurrentState == EGameState::GameWin)
    {
        UpdatePlayerProjectiles(deltaTime);
    }
}

void Game::ProcessInput(float deltaTime, const Input& input)
{
    m_PlayerManager->ProcessInput(deltaTime, input, WIDTH);

    if(m_CurrentState == EGameState::GameWin || m_CurrentState == EGameState::GameOver)
    {
        if(input.GetKey(GLFW_KEY_R))
        {
            Restart();
        }
    }
}

void Game::RenderProjectiles() const
{
    for (auto& projectile : m_PlayerProjectiles)
    {
        projectile.Draw(*m_SpriteRenderer);
    }
    
    for (auto& projectile : m_EnemyProjectiles)
    {
        projectile.Draw(*m_SpriteRenderer);
    }
}

void Game::Render()
{
    m_Level->Render(*m_SpriteRenderer);
    m_PlayerManager->Render(*m_SpriteRenderer);
    
    RenderProjectiles();
    RemoveDestroyedProjectiles();

    if(m_CurrentState == EGameState::Playing)
    {
        m_UIManager->RenderInGameScreen();
    }
    else if(m_CurrentState == EGameState::GameWin)
    {
        m_UIManager->RenderGameWinScreen();
    }
}

void Game::Close()
{
    std::cout << "Closing game...\n";

    ResourceManager::ClearAll();
}

void Game::AddEnemyProjectile(GameObject&& projectile)
{
    m_EnemyProjectiles.emplace_back(std::move(projectile));
}

void Game::AddPlayerProjectile(GameObject&& projectile)
{
    m_PlayerProjectiles.emplace_back(std::move(projectile));
}

void Game::UpdatePlayerProjectiles(float deltaTime)
{
    for (auto& projectile : m_PlayerProjectiles)
    {
        projectile.Position.y -= projectile.Velocity.y * deltaTime;
        
        if (projectile.Position.y <= 0)
        {
            projectile.Destroyed = true;
        }
        else
        {
            CheckEnemyCollisions(projectile);
        }
    }
}

void Game::UpdateEnemyProjectiles(float deltaTime)
{
    GameObject& player = m_PlayerManager->GetPlayer();

    for(auto& projectile : m_EnemyProjectiles)
    {
        projectile.Position += projectile.Velocity * deltaTime;

        if(projectile.Position.y > HEIGHT + projectile.Size.y)
        {
            projectile.Destroyed = true;
        }

        if (player.Destroyed) { continue; }
        if (!Collision::IsColliding(projectile, player)) { continue; }

        projectile.Destroyed = true;
        m_PlayerManager->HandlePlayerHit();
    }
}

void Game::CheckEnemyCollisions(GameObject& projectile)
{
    for (auto& enemy : m_Level->GetEnemies())
    {
        if (enemy.Destroyed) { continue; }
        if (!Collision::IsColliding(projectile, enemy)) { continue; }

        projectile.Destroyed = true;
        m_Level->HandleEnemyHit(enemy);

        break;
    }
}

void Game::HandleGameWon()
{
    m_CurrentState = EGameState::GameWin;
    m_EnemyProjectiles.clear();
}

void Game::Restart()
{
    m_CurrentState = EGameState::Playing;
    m_PlayerProjectiles.clear();
    m_Level->Restart();
}

void Game::RemoveDestroyedProjectiles()
{
    auto playerIterator = m_PlayerProjectiles.begin();
    while (playerIterator != m_PlayerProjectiles.end())
    {
        if (playerIterator->Destroyed)
        {
            playerIterator = m_PlayerProjectiles.erase(playerIterator);
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
        }
        else
        {
            ++enemyIterator;
        }
    }
}
