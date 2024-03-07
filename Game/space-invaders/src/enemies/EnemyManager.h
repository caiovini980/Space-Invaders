#pragma once
#include <map>
#include <vector>

#include "ParticleEmitter.h"
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
    bool IsEveryEnemyKilled() const;
    bool HasEnemyReachedBottom() const;
    void StopAggression();
    void Restart();

    std::vector<GameObject>& GetEnemies() { return m_Enemies; }
    
private:

    static constexpr unsigned int TOTAL_DIFFICULTY_CURVE_POINTS = 5;
    const float DIFFICULTY_CURVE_POINTS[TOTAL_DIFFICULTY_CURVE_POINTS] = {
        0.f,
        0.35f,
        0.6f,
        0.8f,
        0.95f
    };
    const float SHOOT_SECONDS_COOLDOWN_CURVE[TOTAL_DIFFICULTY_CURVE_POINTS] = {
        1.5f,
        1.f,
        0.8f,
        0.6f,
        0.4f
    };
    const float MOVEMENT_VELOCITY_MULTIPLIER_CURVE[TOTAL_DIFFICULTY_CURVE_POINTS] = {
        1.f,
        2.f,
        4.f,
        7.f,
        14.f
    };

    const unsigned int TOTAL_PARTICLE_EMITTERS = 3;
    
    const glm::vec2 INITIAL_MOVEMENT_VELOCITY{20.f, 20.f};
    const glm::vec2 INITIAL_MOVEMENT_DIRECTION{1.f, 0.f};
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
    float m_ShootSecondsCooldown{1.5f};
    float m_LastShootTime{0};
    unsigned int m_CurrentDifficultyIndex{0};
    int m_TotalEnemiesKilled{0};
    int m_TotalEnemies{0};
    std::shared_ptr<Texture> m_ProjectileSprite;
    LevelDefinition m_Level;
    bool bIsPassiveBehaviorEnabled{false};
    bool bHasEnemyReachedBottom{false};

    void MoveEnemies(float deltaTime);
    void MoveEnemiesDownwards();
    bool CanShoot() const;
    void Shoot();
    int GetRandomAliveEnemyIndex() const;
    void IncreaseDifficulty();
    void SpawnEnemies(const LevelDefinition& level);
    glm::vec2 CalculateEnemySize(const LevelDefinition& level);
    glm::vec2 CalculateStartPosition(const LevelDefinition& level);
    
    std::vector<std::shared_ptr<ParticleEmitter>> m_ParticleEmitters;
    unsigned int m_EmitterIndex{0};
};
