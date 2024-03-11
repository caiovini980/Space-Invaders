#pragma once
#include <string>

#include "glm/vec3.hpp"

struct EnemyDefinition
{
    std::string Name;
    std::string SpritePath;
    glm::vec3 Color;
    unsigned int Points;
};
