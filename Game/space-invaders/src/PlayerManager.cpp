#include "PlayerManager.h"

#include <GLFW/glfw3.h>

#include "ResourceManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{ }

void PlayerManager::CreatePlayer(float screenWidth, float screenHeight)
{
    std::cout << "Creating player\n";
    
    const std::string playerTextureName = "Player";
    ResourceManager::LoadTexture("res/textures/awesomeface.png", playerTextureName, true);
    
    m_PlayerSprite = ResourceManager::GetTexture(playerTextureName);
    
    const glm::vec2 playerInitialPosition = glm::vec2(
        (screenWidth / 2.0f) - (PLAYER_SIZE.x / 2.0f),
        screenHeight - PLAYER_SIZE.y);
    
    m_Player = std::make_shared<GameObject>(
        playerInitialPosition,
        PLAYER_SIZE,
        m_PlayerSprite,
        glm::vec3(1.0f),
        glm::vec2(PLAYER_SPEED)
        );
}

void PlayerManager::ProcessInput(float deltaTime, const Input& input)
{
    if (input.GetKey(GLFW_KEY_D))
    {
        std::cout << "Moving right!\n";
    }
    
    else if (input.GetKey(GLFW_KEY_A))
    {
        std::cout << "Moving left!\n";
    }
}

void PlayerManager::Render(SpriteRenderer& renderer) const
{
    renderer.Draw(*m_PlayerSprite, m_Player->Position, m_Player->Size, m_Player->Rotation, m_Player->Color);
}
