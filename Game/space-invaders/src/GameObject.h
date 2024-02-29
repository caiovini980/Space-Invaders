#pragma once
#include <memory>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Texture;
class SpriteRenderer;

class GameObject
{
public:

    glm::vec2 Position{0.f, 0.f};
    glm::vec2 Size{1.f, 1.f};
    glm::vec2 Velocity{0.f};
    glm::vec3 Color{1.f};
    float Rotation{0.f};
    bool Destroyed{false};
    bool bIsPlayer{false};
    
    GameObject();
    GameObject(glm::vec2 position, glm::vec2 size, const std::shared_ptr<Texture>& sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    void Draw(const SpriteRenderer& renderer) const;

private:
    
    std::shared_ptr<Texture> m_Sprite{};
};
