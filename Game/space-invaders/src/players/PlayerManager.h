﻿#pragma once
#include "../GameObject.h"

#include <iostream>

// #include "Game.h"
#include "glm/glm.hpp"

#include "../core/SpriteRenderer.h"
#include "../Input.h"
#include "../interfaces/IProjectileHandler.h"

class PlayerManager
{
public:
    PlayerManager(IProjectileHandler& projectileHandler);
    ~PlayerManager();

    void CreatePlayer(float screenWidth, float screenHeight);
    void CreateProjectile();
    void ProcessInput(float deltaTime, const Input& input, float boundsWidth);
    void Render(const SpriteRenderer& renderer) const;
    void HandlePlayerHit();

    GameObject& GetPlayer() const { return *m_Player; }

private:
    IProjectileHandler& m_ProjectileHandler;
    
    bool CheckIfCanShootAgain() const;
    
    const glm::vec2 PLAYER_SIZE = glm::vec2(60.0f, 60.0f);
    const glm::vec2 PLAYER_OFFSET = glm::vec2(20.0f, 20.0f);

    const glm::vec3 PLAYER_COLOR = glm::vec3(0.5f, 1.0f, 0.5f);

    const float PLAYER_SPEED = 500.0f;
    const float SHOT_COOLDOWN = 1.0f;

    const int PLAYER_INITIAL_LIVES = 3;
    
    std::shared_ptr<GameObject> m_Player;
    std::shared_ptr<Texture> m_PlayerSprite;
    std::shared_ptr<Texture> m_ProjectileSprite;

    glm::vec2 m_ShotPosition;
    
    float m_LastShotTime = 0.0f;

    
    int m_PlayerCurrentLives = 3;
};
