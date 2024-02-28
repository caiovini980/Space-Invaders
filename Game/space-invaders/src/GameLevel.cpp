#include "GameLevel.h"

#include "LevelDefinition.h"


GameLevel::GameLevel(unsigned int width, unsigned int height, IProjectileHandler& projectileHandler)
{
    LevelDefinition levelDefinition{};
    levelDefinition.TotalEnemyColumns = 11;
    levelDefinition.TotalEnemyRows = 5;
    levelDefinition.Padding = 15.f;
    levelDefinition.HorizontalMargin = 80.f;
    levelDefinition.TopMargin = 50.f;
    levelDefinition.MinBottomMargin = 150.f;
    
    m_EnemyManager = std::make_unique<EnemyManager>(width, height, levelDefinition, projectileHandler);
}

void GameLevel::Update(float deltaTime)
{
    m_EnemyManager->Update(deltaTime);
}

void GameLevel::Render(const SpriteRenderer& renderer)
{
    m_EnemyManager->Render(renderer);
}