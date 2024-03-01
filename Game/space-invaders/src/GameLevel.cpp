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
    levelDefinition.GameOverBottomThreshold = static_cast<float>(height) - 120.f;
    
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

void GameLevel::HandleEnemyHit(GameObject& enemy)
{
    m_EnemyManager->HandleEnemyHit(enemy);
}

bool GameLevel::IsEveryEnemyKilled() const
{
    return m_EnemyManager->IsEveryEnemyKilled();
}

bool GameLevel::HasEnemyReachedBottom() const
{
    return m_EnemyManager->HasEnemyReachedBottom();
}

void GameLevel::StopEnemiesAggression() const
{
    m_EnemyManager->StopAggression();
}

void GameLevel::Restart()
{
    m_EnemyManager->Restart();
}
