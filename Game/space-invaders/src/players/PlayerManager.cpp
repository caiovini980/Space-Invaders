﻿#include "PlayerManager.h"

#include <GLFW/glfw3.h>

#include "Audio.h"
#include "../ResourceManager.h"
#include "../utils/GameTime.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

PlayerManager::PlayerManager(IProjectileHandler& projectileHandler)
    : m_ProjectileHandler(projectileHandler)
{
    m_ProjectileSprite = ResourceManager::LoadTexture(
        "res/textures/projectile.png",
        "Projectile",
        true
    );

    m_PlayerShader = ResourceManager::GetShader("Dynamic");
}

PlayerManager::~PlayerManager()
{ }

void PlayerManager::CreatePlayer(float screenWidth, float screenHeight)
{
    std::cout << "Creating player\n";
    
    const std::string playerTextureName = "Player";
    const std::string playerTexturePath = "res/textures/ship.png";
    
    m_PlayerSprite = ResourceManager::LoadTexture(playerTexturePath, playerTextureName, true);
    
    const glm::vec2 playerInitialPosition = CalculateStartPosition(screenWidth, screenHeight);
    
    m_Player = std::make_shared<GameObject>(
        playerInitialPosition,
        PLAYER_SIZE,
        m_PlayerSprite,
        PLAYER_COLOR,
        glm::vec2(PLAYER_SPEED)
        );
    
    m_PlayerCurrentLives = PLAYER_INITIAL_LIVES;
    m_PlayerPreviousPosition = playerInitialPosition;

    const std::shared_ptr<Texture> particleSprite = ResourceManager::GetTexture("PlayerParticle");
    
    m_ParticleEmitter = std::make_unique<ParticleEmitter>(particleSprite);
}

void PlayerManager::ProcessInput(float deltaTime, const Input& input, float boundsWidth)
{
    const float velocity = PLAYER_SPEED * deltaTime;

    if (input.GetKey(GLFW_KEY_A))
    {
        if (m_Player->Position.x >= 0 + PLAYER_OFFSET.x)
        {
            m_Player->Position.x -= velocity; 
        }
    }
    
    else if (input.GetKey(GLFW_KEY_D))
    {
        if (m_Player->Position.x <= boundsWidth - (m_Player->Size.x + PLAYER_OFFSET.x))
        {
            m_Player->Position.x += velocity; 
        }
    }

    if (input.GetKey(GLFW_KEY_SPACE))
    {
        if (CheckIfCanShootAgain())
        {
            CreateProjectile();
            m_LastShotTime = GameTime::Time;
        }
    }
}

void PlayerManager::CreateProjectile()
{
    constexpr glm::vec2 projectileSize = glm::vec2(20.0f, 20.0f);
    constexpr glm::vec3 projectileColor = glm::vec3(0.5f, 1.0f, 1.0f);
    constexpr float projectileSpeed = 800.0f;
    const std::string shotSoundPath = "./res/sounds/laser-shot.mp3";
            
    m_ShotPosition = glm::vec2(
        m_Player->Position.x + (m_Player->Size.x / 2 - 10.0f),
        m_Player->Position.y - 30);
            
    GameObject projectile {
        m_ShotPosition,
        projectileSize,
        m_ProjectileSprite,
        projectileColor,
        glm::vec2(projectileSpeed)
    };

    Audio::Play2DSound(shotSoundPath, false);
            
    m_ProjectileHandler.AddPlayerProjectile(std::move(projectile));
}

void PlayerManager::Render(const SpriteRenderer& renderer) 
{
    m_ParticleEmitter->Render(renderer);

    if (m_Player->Destroyed)
    {
        return;
    }

    m_PlayerShader->Bind();
    glm::mat4 previousModel = glm::mat4{1.f};
    previousModel = glm::translate(previousModel, glm::vec3{m_PlayerPreviousPosition, 0.f});
    previousModel = glm::scale(previousModel, glm::vec3(m_Player->Size, 1.f));
    m_PlayerShader->SetUniformMat4f("u_PreviousModel", previousModel);
    m_PlayerPreviousPosition = m_Player->Position;

    renderer.Draw(*m_PlayerShader, *m_PlayerSprite, m_Player->Position, m_Player->Size, m_Player->Rotation, m_Player->Color);
}

void PlayerManager::Update(float deltaTime)
{
    m_ParticleEmitter->Update(deltaTime);
}

void PlayerManager::HandlePlayerHit()
{
    m_PlayerCurrentLives -= 1;

    m_ParticleEmitter->Emit(*m_Player);
    
    if (m_PlayerCurrentLives <= 0)
    {
        Audio::Play2DSound("./res/sounds/explosion.wav", false, 0.2f);
        m_Player->Destroyed = true;
    }
}

void PlayerManager::DestroyPlayer() const
{
    if(m_Player->Destroyed)
    {
        return;
    }

    m_Player->Destroyed = true;
}

void PlayerManager::Restart(float screenWidth, float screenHeight)
{
    m_Player->Destroyed = false;
    m_Player->Position = CalculateStartPosition(screenWidth, screenHeight);
    m_PlayerPreviousPosition = m_Player->Position;
    m_PlayerCurrentLives = PLAYER_INITIAL_LIVES;
}

bool PlayerManager::CheckIfCanShootAgain() const
{
    return GameTime::Time - m_LastShotTime >= SHOT_COOLDOWN;
}

glm::vec2 PlayerManager::CalculateStartPosition(float screenWidth, float screenHeight)
{
    const glm::vec2 playerInitialPosition = glm::vec2(
            (screenWidth / 2.0f) - (PLAYER_SIZE.x / 2.0f),
            screenHeight - PLAYER_SIZE.y - PLAYER_OFFSET.y);

    return playerInitialPosition;
}
