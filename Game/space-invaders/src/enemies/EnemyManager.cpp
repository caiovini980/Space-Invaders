#include "EnemyManager.h"

#include <iostream>

#include "Audio.h"
#include "EnemyDefinition.h"
#include "IScoreHandler.h"
#include "LevelDefinition.h"
#include "ParticleEmitter.h"
#include "ResourceManager.h"
#include "interfaces/IProjectileHandler.h"
#include "utils/GameTime.h"

EnemyManager::EnemyManager(unsigned levelWidth, unsigned levelHeight, const LevelDefinition& levelDefinition, IProjectileHandler& projectileHandler, IScoreHandler& scoreHandler)
    : m_LevelWidth(levelWidth), m_LevelHeight(levelHeight), m_ProjectileHandler(projectileHandler), m_Level(levelDefinition), m_ScoreHandler(scoreHandler) 
{
    m_Database = std::make_unique<EnemyDatabase>();

    SpawnEnemies(levelDefinition);

    m_ProjectileSprite = ResourceManager::GetTexture("Projectile");
    const std::shared_ptr<Texture> particleSprite = ResourceManager::GetTexture("EnemyParticle");

    m_TotalEnemies = static_cast<int>(m_Enemies.size());
    m_MovementVelocity = INITIAL_MOVEMENT_VELOCITY * MOVEMENT_VELOCITY_MULTIPLIER_CURVE[0];
    m_ShootSecondsCooldown = SHOOT_SECONDS_COOLDOWN_CURVE[0];
    m_MovementDirection = INITIAL_MOVEMENT_DIRECTION;

    for (unsigned int i = 0; i < TOTAL_PARTICLE_EMITTERS; i++)
    {
        m_ParticleEmitters.emplace_back(std::make_shared<ParticleEmitter>(particleSprite));
    }
}

void EnemyManager::Update(float deltaTime)
{
    for (const auto& particleEmmiter : m_ParticleEmitters)
    {
        particleEmmiter->Update(deltaTime);
    }
    
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
        if(enemy.Destroyed)
        {
            continue;
        }
        
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

    if(!bIsPassiveBehaviorEnabled)
    {
        MoveEnemiesDownwards();
    }
}

void EnemyManager::MoveEnemiesDownwards()
{
    for(GameObject& enemy : m_Enemies)
    {
        if(enemy.Destroyed)
        {
            continue;
        }

        enemy.Position.y += m_EnemySize.y / 2.f;

        if(enemy.Position.y >= m_Level.GameOverBottomThreshold)
        {
            bHasEnemyReachedBottom = true;
        }
    }
}

bool EnemyManager::CanShoot() const
{
    return !bIsPassiveBehaviorEnabled && GameTime::Time - m_LastShootTime >= m_ShootSecondsCooldown;
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
        for (const auto& particleEmmiter : m_ParticleEmitters)
        {
            particleEmmiter->Render(renderer);
        }
        
        if (enemy.Destroyed) continue;
        enemy.Draw(renderer);
    }
}

void EnemyManager::HandleEnemyHit(Enemy& enemy)
{
    m_TotalEnemiesKilled++;
    enemy.Destroyed = true;

    Audio::Play2DSound("./res/sounds/explosion.wav", false, 0.2f);

    m_ParticleEmitters[m_EmitterIndex]->Emit(enemy);
    m_EmitterIndex = (m_EmitterIndex + 1) % TOTAL_PARTICLE_EMITTERS;

    m_ScoreHandler.AddScore(enemy.GetPoints());
    
    IncreaseDifficulty();
}

void EnemyManager::IncreaseDifficulty()
{
    unsigned int nextDifficultyIndex = m_CurrentDifficultyIndex + 1;

    if(nextDifficultyIndex == TOTAL_DIFFICULTY_CURVE_POINTS)
    {
        return;
    }

    float killedEnemiesCompletion = static_cast<float>(m_TotalEnemiesKilled) / static_cast<float>(m_TotalEnemies);

    if(killedEnemiesCompletion < DIFFICULTY_CURVE_POINTS[nextDifficultyIndex])
    {
        return;
    }

    m_CurrentDifficultyIndex = nextDifficultyIndex;

    m_ShootSecondsCooldown = SHOOT_SECONDS_COOLDOWN_CURVE[m_CurrentDifficultyIndex];
    m_MovementVelocity = INITIAL_MOVEMENT_VELOCITY * MOVEMENT_VELOCITY_MULTIPLIER_CURVE[m_CurrentDifficultyIndex];
}

void EnemyManager::StopAggression()
{
    bIsPassiveBehaviorEnabled = true;
}

bool EnemyManager::IsEveryEnemyKilled() const
{
    return m_TotalEnemiesKilled >= m_TotalEnemies;
}

bool EnemyManager::HasEnemyReachedBottom() const
{
    return bHasEnemyReachedBottom;
}

void EnemyManager::Restart()
{
    m_TotalEnemiesKilled = 0;
    m_CurrentDifficultyIndex = 0;
    m_MovementVelocity = INITIAL_MOVEMENT_VELOCITY * MOVEMENT_VELOCITY_MULTIPLIER_CURVE[0];
    m_ShootSecondsCooldown = SHOOT_SECONDS_COOLDOWN_CURVE[0];
    m_LastShootTime = GameTime::Time;
    m_MovementDirection = INITIAL_MOVEMENT_DIRECTION;
    bIsPassiveBehaviorEnabled = false;
    bHasEnemyReachedBottom = false;

    glm::vec2 startPosition = CalculateStartPosition(m_Level);
    int i = 0;
    
    for(unsigned int y = 0; y < m_Level.TotalEnemyRows; y++)
    {
        for(unsigned int x = 0; x < m_Level.TotalEnemyColumns; x++)
        {
            std::shared_ptr<EnemyDefinition> definition = m_Database->GetDefinition(m_Level.EnemyIds[y][x]);

            if(!definition)
            {
                continue;
            }

            glm::vec2 position = startPosition;
            position.x += (m_Level.Padding + m_EnemySize.x) * x;
            position.y += (m_Level.Padding + m_EnemySize.y) * y;

            m_Enemies[i].Position = position;
            m_Enemies[i].Destroyed = false;

            i++;
        }
    }
}

void EnemyManager::SpawnEnemies(const LevelDefinition& level)
{
    m_EnemySize = CalculateEnemySize(level);
    glm::vec2 startPosition = CalculateStartPosition(level);

    for(unsigned int y = 0; y < level.TotalEnemyRows; y++)
    {
        for(unsigned int x = 0; x < level.TotalEnemyColumns; x++)
        {
            std::shared_ptr<EnemyDefinition> definition = m_Database->GetDefinition(level.EnemyIds[y][x]);

            if(!definition)
            {
                continue;
            }
            
            glm::vec2 position = startPosition;
            position.x += (level.Padding + m_EnemySize.x) * x;
            position.y += (level.Padding + m_EnemySize.y) * y;

            std::shared_ptr<Texture> enemySprite = ResourceManager::LoadTexture(definition->SpritePath, definition->Name, true);
            m_Enemies.emplace_back(*definition, position, m_EnemySize, enemySprite);
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

glm::vec2 EnemyManager::CalculateStartPosition(const LevelDefinition& level)
{
    float rowWidth = m_EnemySize.x * level.TotalEnemyColumns + level.Padding * level.TotalEnemyColumns;
    glm::vec2 startPosition = {m_LevelWidth / 2.f - rowWidth / 2.f , level.TopMargin};

    return startPosition;
}
