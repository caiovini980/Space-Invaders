#include "EnemyManager.h"

#include "LevelDefinition.h"
#include "ResourceManager.h"

EnemyManager::EnemyManager(unsigned levelWidth, unsigned levelHeight, const LevelDefinition& levelDefinition)
    : m_LevelWidth(levelWidth), m_LevelHeight(levelHeight)
{
    SpawnEnemies(levelDefinition);
}

void EnemyManager::Update(float deltaTime)
{ }

void EnemyManager::Render(const SpriteRenderer& renderer)
{
    for (const GameObject& enemy : m_Enemies)
    {
        enemy.Draw(renderer);
    }
}

void EnemyManager::SpawnEnemies(const LevelDefinition& level)
{
    std::shared_ptr<Texture> enemySprite = ResourceManager::LoadTexture("res/textures/enemy.png", "Enemy", true);
    
    glm::vec2 enemySize = CalculateEnemySize(level);
    
    float rowWidth = enemySize.x * level.TotalEnemyColumns + level.Padding * level.TotalEnemyColumns;
    glm::vec2 startPosition = {m_LevelWidth / 2.f - rowWidth / 2.f , level.TopMargin};

    std::vector<glm::vec3> colorMapping{
        glm::vec3{0.7f, 0.f, 0.7f},
        glm::vec3{0.7f, 0.7f, 0.f},
        glm::vec3{0.f, 0.7f, 0.7f}
    };

    int totalColors = static_cast<int>(colorMapping.size());
    int colorIndex = 0;
    const int rowsPerColor = 2;

    for(unsigned int y = 0; y < level.TotalEnemyRows; y++)
    {
        for(unsigned int x = 0; x < level.TotalEnemyColumns; x++)
        {
            glm::vec2 position = startPosition;
            position.x += (level.Padding + enemySize.x) * x;
            position.y += (level.Padding + enemySize.y) * y;
            
            m_Enemies.emplace_back(position, enemySize, enemySprite, colorMapping[colorIndex]);
        }

        if(y % rowsPerColor == 0)
        {
            colorIndex = (colorIndex + 1) % totalColors;
        }
    }
}

glm::vec2 EnemyManager::CalculateEnemySize(const LevelDefinition& level)
{
    const float maxSize = 40.f;
    
    float desiredWidth = (m_LevelWidth - level.Padding * level.TotalEnemyColumns - level.HorizontalMargin * 2) / static_cast<float>(level.TotalEnemyColumns);
    float desiredHeight = (m_LevelHeight - level.Padding * level.TotalEnemyRows - level.TopMargin - level.MinBottomMargin ) / static_cast<float>(level.TotalEnemyRows);

    float smallestSide = std::min(desiredWidth, desiredHeight);
    
    if(smallestSide > maxSize)
    {
        smallestSide = maxSize;
    }

    desiredWidth = smallestSide;
    desiredHeight = smallestSide;
    
    return glm::vec2{desiredWidth, desiredHeight};
}
