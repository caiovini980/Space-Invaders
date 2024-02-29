#include "BackgroundManager.h"

#include "GameObject.h"
#include "ResourceManager.h"

BackgroundManager::BackgroundManager(unsigned levelWidth, unsigned levelHeight)
{
    // load texture
    m_GameBackgroundSprite = ResourceManager::LoadTexture("./res/textures/background.png", "Background", false);

    // set size
    glm::vec2 gameBackgroundSize = glm::vec2(levelWidth, levelHeight);

    // set position
    glm::vec2 gameBackgroundPosition = glm::vec2(0, 0);
    
    // set color
    glm::vec3 gameBackgroundColor = glm::vec3(1.0, 0.0, 0.5);

    // create GameObject
    m_GameBackground = std::make_shared<GameObject>(
        gameBackgroundPosition,
        gameBackgroundSize,
        m_GameBackgroundSprite,
        gameBackgroundColor);
}

void BackgroundManager::Render(SpriteRenderer& renderer)
{
    m_GameBackground->Draw(renderer);
}

void BackgroundManager::Update(float deltaTime)
{
    
}
