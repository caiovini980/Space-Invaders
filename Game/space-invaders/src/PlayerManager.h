#pragma once
#include "GameObject.h"

#include <iostream>

#include "glm/glm.hpp"

#include "Input.h"
#include "SpriteRenderer.h"

class PlayerManager
{
public:
    PlayerManager();
    ~PlayerManager();

    void CreatePlayer(float screenWidth, float screenHeight);
    void ProcessInput(float deltaTime, const Input& input, float boundsWidth);
    void Render(const SpriteRenderer& renderer) const;

private:
    const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 100.0f);
    const float PLAYER_SPEED = 500.0f;
    
    std::shared_ptr<GameObject> m_Player;
    std::shared_ptr<Texture> m_PlayerSprite;
};
