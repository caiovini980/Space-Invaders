#include "BackgroundManager.h"

#include "GameObject.h"
#include "ResourceManager.h"

BackgroundManager::BackgroundManager(unsigned levelWidth, unsigned levelHeight)
{
    m_GameBackgroundSprite = ResourceManager::LoadTexture("./res/textures/background.png", "Background", false);

    const glm::vec2 gameBackgroundSize = glm::vec2(levelWidth, levelHeight);
    constexpr glm::vec2 gameBackgroundPosition = glm::vec2(0, 0);
    constexpr glm::vec3 gameBackgroundColor = glm::vec3(1.0, 0.0, 0.5);
    
    m_GameBackground = GameObject {
        gameBackgroundPosition,
        gameBackgroundSize,
        m_GameBackgroundSprite,
        gameBackgroundColor
    };
}

auto BackgroundManager::Render(const SpriteRenderer& renderer) const -> void
{
    m_GameBackground.Draw(renderer);
}

void BackgroundManager::Update(float deltaTime)
{
    
}
