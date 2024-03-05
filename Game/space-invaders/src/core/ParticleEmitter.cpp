#include "ParticleEmitter.h"

#include "ResourceManager.h"
#include "utils/GameTime.h"
#include "utils/Random.h"

ParticleEmitter::ParticleEmitter(const char* spritePath)
{
    m_ParticleSprite = ResourceManager::LoadTexture(spritePath, "PlayerParticle", true);
    
    // spawn particles
    SpawnParticles();
}

void ParticleEmitter::Update(float deltaTime)
{
    // move the particles
    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        m_ParticleProps[i].Object.Position += m_ParticleProps[i].Speed * deltaTime * m_ParticleProps[i].Direction;
        
        if (GameTime::Time - m_ParticleProps[i].SpawnedTime >= m_ParticleProps[i].TimeToFadeAway)
        {
            m_ParticleProps[i].Object.Destroyed = true;
        }
    }
}

void ParticleEmitter::Render(const SpriteRenderer& renderer)
{
    // draw particles
    for (const auto& particle : m_ParticleProps)
    {
        if (!particle.Object.Destroyed)
        {
            renderer.Draw(*m_ParticleSprite, particle.Object.Position, particle.Object.Size, particle.Object.Rotation, particle.Object.Color);
        }
    }
}

void ParticleEmitter::Emit(const GameObject& source)
{
    // reset position
    // show particles
    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        m_ParticleProps[i].SpawnedTime = GameTime::Time;
        m_ParticleProps[i].Direction = GetRandomDirectionToParticle();
        m_ParticleProps[i].Object.Destroyed = false;
        m_ParticleProps[i].Object.Position = glm::vec2(
            source.Position.x + (source.Size.x / 2 - source.Size.x / 4),
            source.Position.y + (source.Size.y / 2 - source.Size.y / 4));
    }
}

glm::vec2 ParticleEmitter::GetRandomDirectionToParticle()
{
    return glm::vec2(2 * Random::Float() - 0.5, 2 * Random::Float() - 0.5);
}

void ParticleEmitter::SpawnParticles()
{
    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        constexpr glm::vec2 size = glm::vec2(20.0f, 20.0f);
        constexpr glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);
        constexpr float speed = 80.0f;
        glm::vec2 position = glm::vec2(1000.f); // outside the screen
            
        GameObject newParticle {
            position,
            size,
            m_ParticleSprite,
            color
        };

        ParticleProps prop;
        prop.Object = newParticle;
        prop.Direction = GetRandomDirectionToParticle();
        prop.SpawnedTime = GameTime::Time;
        prop.TimeToFadeAway = Random::Float() * 2;
        prop.Speed = Random::Float() * 50;

        m_ParticleProps.emplace_back(prop);
    }
}
