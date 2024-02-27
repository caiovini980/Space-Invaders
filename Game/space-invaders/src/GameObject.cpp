#include "GameObject.h"

#include "SpriteRenderer.h"

GameObject::GameObject()
{ }

GameObject::GameObject(glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(position), Size(size), Velocity(velocity), Color(color), m_Sprite(sprite) 
{
}

void GameObject::Draw(const SpriteRenderer& renderer) const
{
    renderer.Draw(*m_Sprite, Position, Size, Rotation, Color);
}
