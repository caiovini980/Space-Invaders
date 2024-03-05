#pragma once
#include "GameObject.h"

class Barrier : public GameObject
{
public:

    Barrier(glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite, int totalLives);

    void HandleHit();
    void Restart();
    
    inline int GetTotalLives() const { return m_CurrentLives; }

private:

    static constexpr glm::vec3 BARRIER_COLOR{0.22f, 0.92f, 0.04f};

    int m_MaxLives{1};
    int m_CurrentLives{1};
};
