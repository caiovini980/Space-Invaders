#pragma once
#include "EnemyDefinition.h"
#include "GameObject.h"

struct EnemyDefinition;

class Enemy : public GameObject
{
public:

    Enemy(const EnemyDefinition& definition, glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite);

    inline const EnemyDefinition& GetDefinition() const { return m_Definition; }
    inline unsigned int GetPoints() const { return m_Definition.Points; }

private:

    const EnemyDefinition& m_Definition;
};
