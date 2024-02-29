#pragma once
#include <memory>

#include "GameObject.h"
#include "Texture.h"

class SpriteRenderer;
class GameObject;

class BackgroundManager
{
public:
    BackgroundManager(unsigned int levelWidth, unsigned int levelHeight);
    
    void Render(const SpriteRenderer& renderer) const;
    void Update(float deltaTime);

private:
    GameObject m_GameBackground;
    std::shared_ptr<Texture> m_GameBackgroundSprite;
};
