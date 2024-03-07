#include "ParticleEmitter.h"

#include "ResourceManager.h"
#include "utils/GameTime.h"
#include "utils/Random.h"

ParticleEmitter::ParticleEmitter(const std::shared_ptr<Texture>& sprite)
    : m_ParticleSprite(sprite)
{
    SpawnParticles();
}

void ParticleEmitter::Update(float deltaTime)
{
    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        m_ParticleProps[i].Position += m_ParticleProps[i].Speed * deltaTime * m_ParticleProps[i].Direction;
        m_ParticleProps[i].Lifetime -= deltaTime;
    }
}

void ParticleEmitter::Render(const SpriteRenderer& renderer)
{
    for (const auto& particle : m_ParticleProps)
    {
        if (particle.Lifetime > 0.f)
        {
            renderer.Draw(*m_ParticleSprite, particle.Position, particle.Size, particle.Rotation, particle.Color);
        }
    }
}

void ParticleEmitter::Emit(const GameObject& source)
{
    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        m_ParticleProps[i].Lifetime = Random::Float() * 2;
        m_ParticleProps[i].Direction = GetRandomDirectionToParticle();
        m_ParticleProps[i].Position = glm::vec2(
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
    constexpr glm::vec2 size = glm::vec2(20.0f, 20.0f);
    constexpr glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);
    constexpr glm::vec2 outsideScreenPosition = glm::vec2(1000.f);
    constexpr glm::vec2 defaultDirection = glm::vec2(0.f, 0.f);

    m_ParticleProps.reserve(m_AmountOfParticles);

    for (int i = 0; i < m_AmountOfParticles; i++)
    {
        ParticleProps prop;
        prop.Position = outsideScreenPosition;
        prop.Size = size;
        prop.Color = color;
        prop.Direction = defaultDirection;
        prop.Lifetime = 0.f;
        prop.Speed = Random::Float() * 50;

        m_ParticleProps.emplace_back(prop);
    }
}
