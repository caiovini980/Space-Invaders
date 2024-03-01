#include "Barrier.h"

Barrier::Barrier(glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite, int totalLives)
    : GameObject(position, size, sprite, BARRIER_COLOR), m_MaxLives(totalLives), m_CurrentLives(totalLives)
{ }

void Barrier::HandleHit()
{
    if(Destroyed)
    {
        return;
    }
    
    m_CurrentLives--;

    if(m_CurrentLives <= 0)
    {
        Destroyed = true;
    }
}

void Barrier::Restart()
{
    Destroyed = false;
    m_CurrentLives = m_MaxLives;
}
