#include "GameLevel.h"

#include "ResourceManager.h"

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
    levelDefinition.TotalBarriers = 4;
    levelDefinition.MaxBarrierWidth = 100.f;
    levelDefinition.BarrierPadding = 400.f / static_cast<float>(levelDefinition.TotalBarriers);
    levelDefinition.BarrierHorizontalMargin = 40.f;
    levelDefinition.BarrierBottomMargin = 100.f;
    
    m_EnemyManager = std::make_unique<EnemyManager>(width, height, levelDefinition, projectileHandler);

    SpawnBarriers(static_cast<float>(width), static_cast<float>(height), levelDefinition);
}

void GameLevel::Update(float deltaTime)
{
    m_EnemyManager->Update(deltaTime);
}

void GameLevel::Render(const SpriteRenderer& renderer)
{
    for(const GameObject& barrier : m_Barriers)
    {
        barrier.Draw(renderer);
    }

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

void GameLevel::SpawnBarriers(float screenWidth, float screenHeight, const LevelDefinition& level)
{
    if(level.TotalBarriers == 0)
    {
        return;
    }
    
    std::shared_ptr<Texture> sprite = ResourceManager::LoadTexture("res/textures/block.png", "Barrier", true);
    float spriteAspectRatio = static_cast<float>(sprite->GetWidth()) / static_cast<float>(sprite->GetHeight());
    
    glm::vec2 size{0.f};
    size.x = (screenWidth - level.BarrierPadding * static_cast<float>(level.TotalBarriers) - level.BarrierHorizontalMargin * 2.f) / static_cast<float>(level.TotalBarriers);
    size.x = std::min(size.x, level.MaxBarrierWidth);
    size.y = size.x / spriteAspectRatio;

    float rowWidth = size.x * static_cast<float>(level.TotalBarriers) + level.BarrierPadding * static_cast<float>(level.TotalBarriers);
    glm::vec2 startPosition = {screenWidth / 2.f - rowWidth / 2.f + level.BarrierPadding / 2.f, screenHeight - level.BarrierBottomMargin - size.y};

    m_Barriers.reserve(level.TotalBarriers);

    for(int i = 0; i < level.TotalBarriers; i++)
    {
        glm::vec2 position = startPosition;
        position.x += (level.BarrierPadding + size.x) * static_cast<float>(i);
        m_Barriers.emplace_back(position, size, sprite, glm::vec3{0.f, 1.f, 0.f});
    }
}
