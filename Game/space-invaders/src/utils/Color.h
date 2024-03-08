#pragma once
#include <string>

#include "glm/vec3.hpp"

class Color
{
public:
    static glm::vec3 HexToRGB(const std::string& hexColor);

private:

    static float HexToFloat(const std::string& hexComponentCode);
};
