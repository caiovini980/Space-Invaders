#pragma once
#include <memory>

#include "LevelDefinition.h"
#include "enemies/EnemyManager.h"
#include "level/Barrier.h"

class Shader;
class IProjectileHandler;
class SpriteRenderer;

class GameLevel
{
public:

    GameLevel(unsigned int width, unsigned int height, IProjectileHandler& projectileHandler);
    
    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);

    std::vector<GameObject>& GetEnemies() const { return m_EnemyManager->GetEnemies(); }
    std::vector<Barrier>& GetBarriers() { return m_Barriers; }

    void HandleEnemyHit(GameObject& projectile);
    bool IsEveryEnemyKilled() const;
    bool HasEnemyReachedBottom() const;
    void StopEnemiesAggression() const;
    void HandleBarrierHit(Barrier& barrier);
    void Restart();

private:

    static constexpr int BARRIER_TOTAL_LIVES = 10;
    
    std::unique_ptr<EnemyManager> m_EnemyManager;
    std::vector<Barrier> m_Barriers{};
    std::shared_ptr<Shader> m_BarrierShader;
    std::shared_ptr<Texture> m_BarrierBreakingTexture;

    void SpawnBarriers(float screenWidth, float screenHeight, const LevelDefinition& level);
};
