#pragma once
#include <memory>

#include "enemies/EnemyManager.h"

class SpriteRenderer;

class GameLevel
{
public:

    GameLevel(unsigned int width, unsigned int height);
    
    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);

private:

    std::unique_ptr<EnemyManager> m_EnemyManager;
};
