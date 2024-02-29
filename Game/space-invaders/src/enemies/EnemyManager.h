#pragma once
#include <vector>

#include "../GameObject.h"
#include "../LevelDefinition.h"

class IProjectileHandler;
class SpriteRenderer;

class EnemyManager
{
public:

    EnemyManager(unsigned int levelWidth, unsigned int levelHeight, const LevelDefinition& levelDefinition, IProjectileHandler& projectileHandler);
    
    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);
    void HandleEnemyHit(GameObject& enemy);

    std::vector<GameObject>& GetEnemies() { return m_Enemies; }
    
private:

    const float SHOOT_SECONDS_COOLDOWN = 1.5f;
    const glm::vec2 SHOOT_VELOCITY{0.f, 300.f};
    const glm::vec2 PROJECTILE_SIZE{20.f, 20.f};
    const glm::vec3 PROJECTILE_COLOR{1.f, 0.f, 0.f};
    
    unsigned int m_LevelWidth{0};
    unsigned int m_LevelHeight{0};
    IProjectileHandler& m_ProjectileHandler;
    std::vector<GameObject> m_Enemies{};
    glm::vec2 m_EnemySize{40.f, 40.f};
    glm::vec2 m_MovementDirection{1.f, 0.f};
    glm::vec2 m_MovementVelocity{20.f, 20.f};
    float m_LastShootTime{0};
    std::shared_ptr<Texture> m_ProjectileSprite;

    void MoveEnemies(float deltaTime);
    void MoveEnemiesDownwards();
    bool CanShoot() const;
    void Shoot();
    int GetRandomAliveEnemyIndex() const;
    void SpawnEnemies(const LevelDefinition& level);
    glm::vec2 CalculateEnemySize(const LevelDefinition& level);
};
