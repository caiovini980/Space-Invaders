#include "EnemyManager.h"

#include <iostream>

#include "LevelDefinition.h"
#include "ResourceManager.h"
#include "interfaces/IProjectileHandler.h"
#include "utils/GameTime.h"

EnemyManager::EnemyManager(unsigned levelWidth, unsigned levelHeight, const LevelDefinition& levelDefinition, IProjectileHandler& projectileHandler)
    : m_LevelWidth(levelWidth), m_LevelHeight(levelHeight), m_ProjectileHandler(projectileHandler)
{
    SpawnEnemies(levelDefinition);

    m_ProjectileSprite = ResourceManager::GetTexture("Projectile");
}

void EnemyManager::Update(float deltaTime)
{
    MoveEnemies(deltaTime);

    if(CanShoot())
    {
        Shoot();
    }
}

void EnemyManager::MoveEnemies(float deltaTime)
{
    bool bHasHitWall = false;

    for(GameObject& enemy : m_Enemies)
    {
        enemy.Position += m_MovementVelocity * m_MovementDirection * deltaTime;

        bool bIsCrossingLeftBound = m_MovementDirection.x < 0.f && enemy.Position.x <= 0.f;
        bool bIsCrossingRightBound = m_MovementDirection.x > 0.f && enemy.Position.x + m_EnemySize.x >= m_LevelWidth;
        
        if(bIsCrossingLeftBound || bIsCrossingRightBound)
        {
            bHasHitWall = true;
        }
    }

    if(!bHasHitWall)
    {
        return;
    }

    m_MovementDirection.x = -m_MovementDirection.x;

    MoveEnemiesDownwards();
}

void EnemyManager::MoveEnemiesDownwards()
{
    for(GameObject& enemy : m_Enemies)
    {
        enemy.Position.y += m_EnemySize.y / 2.f;
    }
}

bool EnemyManager::CanShoot() const
{
    return GameTime::Time - m_LastShootTime >= SHOOT_SECONDS_COOLDOWN;
}

void EnemyManager::Shoot()
{
    m_LastShootTime = GameTime::Time;

    int randomEnemyIndex = GetRandomAliveEnemyIndex();

    if(randomEnemyIndex == -1)
    {
        std::cout << "ENEMY::ERROR: Could not found a enemy to shoot from!\n";
        return;
    }

    const GameObject& enemy = m_Enemies[randomEnemyIndex];
    
    glm::vec2 shootOrigin{enemy.Position.x + enemy.Size.x / 2.f, enemy.Position.y + enemy.Size.y};
    GameObject projectile{shootOrigin, PROJECTILE_SIZE, m_ProjectileSprite, PROJECTILE_COLOR, SHOOT_VELOCITY};
    projectile.Rotation = 180.f;

    m_ProjectileHandler.AddEnemyProjectile(std::move(projectile));
}

int EnemyManager::GetRandomAliveEnemyIndex() const
{
    constexpr int maxAttempts = 1000;
    int i = 0;

    while(i < maxAttempts)
    {
        int randomIndex = rand() % m_Enemies.size();

        if(!m_Enemies[randomIndex].Destroyed)
        {
            return randomIndex;
        }

        i++;
    }
    
    return -1;
}

void EnemyManager::Render(const SpriteRenderer& renderer)
{
    for(const GameObject& enemy : m_Enemies)
    {
        enemy.Draw(renderer);
    }
}

void EnemyManager::SpawnEnemies(const LevelDefinition& level)
{
    std::shared_ptr<Texture> enemySprite = ResourceManager::LoadTexture("res/textures/enemy.png", "Enemy", true);
    
    m_EnemySize = CalculateEnemySize(level);
    
    float rowWidth = m_EnemySize.x * level.TotalEnemyColumns + level.Padding * level.TotalEnemyColumns;
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
            position.x += (level.Padding + m_EnemySize.x) * x;
            position.y += (level.Padding + m_EnemySize.y) * y;
            
            m_Enemies.emplace_back(position, m_EnemySize, enemySprite, colorMapping[colorIndex]);
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
