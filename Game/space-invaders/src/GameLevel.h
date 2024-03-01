#pragma once
#include <memory>

#include "LevelDefinition.h"
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
    void HandleEnemyHit(GameObject& projectile);
    bool IsEveryEnemyKilled() const;
    bool HasEnemyReachedBottom() const;
    void StopEnemiesAggression() const;
    void Restart();

private:

    std::unique_ptr<EnemyManager> m_EnemyManager;
    std::vector<GameObject> m_Barriers{};

    void SpawnBarriers(float screenWidth, float screenHeight, const LevelDefinition& level);
};
