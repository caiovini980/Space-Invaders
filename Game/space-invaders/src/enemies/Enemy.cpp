#include "Enemy.h"

#include "EnemyDefinition.h"

Enemy::Enemy(const EnemyDefinition& definition, glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite)
    : GameObject(position, size, sprite, definition.Color), m_Definition(definition)
{ }
