#include "GameLevel.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "ResourceManager.h"
#include "glm/ext/matrix_clip_space.hpp"

GameLevel::GameLevel(unsigned int width, unsigned int height, IProjectileHandler& projectileHandler, IScoreHandler& scoreHandler)
{
    LevelDefinition levelDefinition = Load("res/data/levels/1.level", width, height);
    
    m_EnemyManager = std::make_unique<EnemyManager>(width, height, levelDefinition, projectileHandler, scoreHandler);
    
    SpawnBarriers(static_cast<float>(width), static_cast<float>(height), levelDefinition);

    m_BarrierShader = ResourceManager::LoadShader("res/shaders/Sprite.vertex", "res/shaders/SpriteBreaking.frag", "Barrier");
    m_BarrierShader->Bind();
    glm::mat4 ortho = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);
    m_BarrierShader->SetUniformMat4f("u_Projection", ortho);
    m_BarrierShader->SetUniform1i("u_Image", 0);
    m_BarrierShader->SetUniform1i("u_BreakingImage", 1);

    m_BarrierBreakingTexture = ResourceManager::LoadTexture("res/textures/block_breaking.png", "BarrierBreaking", true);
}

void GameLevel::Update(float deltaTime)
{
    m_EnemyManager->Update(deltaTime);
}

void GameLevel::Render(const SpriteRenderer& renderer)
{
    m_BarrierShader->Bind();
    m_BarrierBreakingTexture->Bind(1);

    for(const Barrier& barrier : m_Barriers)
    {
        if(barrier.Destroyed)
        {
            continue;
        }

        const float integrity = static_cast<float>(barrier.GetTotalLives()) / static_cast<float>(BARRIER_TOTAL_LIVES);
        m_BarrierShader->SetUniform1f("u_Integrity", integrity);
        
        barrier.Draw(renderer, *m_BarrierShader);
    }

    m_EnemyManager->Render(renderer);
}

void GameLevel::HandleEnemyHit(Enemy& enemy)
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

void GameLevel::HandleBarrierHit(Barrier& barrier)
{
    barrier.HandleHit();
}

void GameLevel::Restart()
{
    m_EnemyManager->Restart();

    for (Barrier& barrier : m_Barriers)
    {
        barrier.Restart();
    }
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

    for(unsigned int i = 0; i < level.TotalBarriers; i++)
    {
        glm::vec2 position = startPosition;
        position.x += (level.BarrierPadding + size.x) * static_cast<float>(i);
        m_Barriers.emplace_back(position, size, sprite, BARRIER_TOTAL_LIVES);
    }
}

LevelDefinition GameLevel::Load(const char* levelFilePath, unsigned int width, unsigned int height)
{
    LevelDefinition levelDefinition{};
    levelDefinition.TotalEnemyColumns = 0;
    levelDefinition.TotalEnemyRows = 0;
    levelDefinition.Padding = 15.f;
    levelDefinition.HorizontalMargin = 80.f;
    levelDefinition.TopMargin = 50.f;
    levelDefinition.MinBottomMargin = 150.f;
    levelDefinition.GameOverBottomThreshold = static_cast<float>(height) - 120.f;
    levelDefinition.TotalBarriers = 4;
    levelDefinition.MaxBarrierWidth = 100.f;
    levelDefinition.BarrierPadding = levelDefinition.TotalBarriers > 0 ? 400.f / static_cast<float>(levelDefinition.TotalBarriers) : 0.f;
    levelDefinition.BarrierHorizontalMargin = 40.f;
    levelDefinition.BarrierBottomMargin = 100.f;

    std::ifstream fileStream(levelFilePath);
    assert(fileStream);
    
    std::string line;
    unsigned int enemyId;

    while(std::getline(fileStream, line))
    {
        std::istringstream stringStream(line);
        std::vector<unsigned int> row;
        unsigned int totalColumns = 0;

        while(stringStream >> enemyId)
        {
            totalColumns++;
            row.push_back(enemyId);
        }

        levelDefinition.EnemyIds.emplace_back(std::move(row));
        levelDefinition.TotalEnemyRows++;

        if(totalColumns > levelDefinition.TotalEnemyColumns)
        {
            levelDefinition.TotalEnemyColumns = totalColumns;            
        }
    }

    return levelDefinition;
}
