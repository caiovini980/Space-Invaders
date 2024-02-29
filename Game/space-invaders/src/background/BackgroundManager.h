#pragma once
#include <memory>

#include "Texture.h"

class SpriteRenderer;
class GameObject;

class BackgroundManager
{
public:
    BackgroundManager(unsigned int levelWidth, unsigned int levelHeight);
    
    void Render(SpriteRenderer& renderer);
    void Update(float deltaTime);

private:
    std::shared_ptr<GameObject> m_GameBackground;
    std::shared_ptr<Texture> m_GameBackgroundSprite;
};
