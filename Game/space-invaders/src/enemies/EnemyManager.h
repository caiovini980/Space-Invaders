#pragma once
#include <vector>

#include "GameObject.h"
#include "LevelDefinition.h"

class SpriteRenderer;

class EnemyManager
{
public:

    EnemyManager(unsigned int levelWidth, unsigned int levelHeight, const LevelDefinition& levelDefinition);
    
    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);

private:

    unsigned int m_LevelWidth{0};
    unsigned int m_LevelHeight{0};
    std::vector<GameObject> m_Enemies{};
    glm::vec2 m_EnemySize{40.f, 40.f};
    glm::vec2 m_MovementDirection{1.f, 0.f};
    glm::vec2 m_MovementVelocity{20.f, 20.f};

    void MoveEnemies(float deltaTime);
    void MoveEnemiesDownwards();
    void SpawnEnemies(const LevelDefinition& level);
    glm::vec2 CalculateEnemySize(const LevelDefinition& level);
};
