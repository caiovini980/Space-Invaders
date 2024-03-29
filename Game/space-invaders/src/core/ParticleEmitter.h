﻿#pragma once
#include <memory>
#include <vector>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "glm/glm.hpp"

class ParticleEmitter
{
public:
    ParticleEmitter(const std::shared_ptr<Texture>& sprite);

    void Update(float deltaTime);
    void Render(const SpriteRenderer& renderer);
    void Emit(const GameObject& source);
    
private:
    struct ParticleProps
    {
        glm::vec2 Position;
        glm::vec2 Size;
        float Rotation;
        glm::vec3 Color;
        glm::vec2 Direction;
        float Lifetime;
        float Speed;
    };

    inline glm::vec2 GetRandomDirectionToParticle();
    void SpawnParticles();
    
    std::vector<ParticleProps> m_ParticleProps;
    
    std::shared_ptr<Texture> m_ParticleSprite;

    int m_AmountOfParticles { 5 };
};

