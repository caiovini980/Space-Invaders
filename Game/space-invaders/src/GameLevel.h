#pragma once
#include <memory>

#include "enemies/EnemyManager.h"

class IProjectileHandler;
class SpriteRenderer;

class GameLevel
{
public:

    GameLevel(unsigned int width, unsigned int height, IProjectileHandler& projectileHandler);
    
    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);

    std::vector<GameObject>& GetEnemies() const { return m_EnemyManager->GetEnemies(); }
    void CheckCollisionWithEnemies(GameObject& projectile, GameObject& object);

private:

    std::unique_ptr<EnemyManager> m_EnemyManager;
};
