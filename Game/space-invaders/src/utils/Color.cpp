#include "Color.h"

#include <sstream>

glm::vec3 Color::HexToRGB(const std::string& hexColor)
{
    std::string hex = hexColor.substr(hexColor.find_first_not_of('#'));

    glm::vec3 color{0.f};
    color.r = HexToFloat(hex.substr(0, 2)) / 255.f;
    color.g = HexToFloat(hex.substr(2, 2)) / 255.f;
    color.b = HexToFloat(hex.substr(4, 2)) / 255.f;

    return color;
}

float Color::HexToFloat(const std::string& hexComponentCode)
{
    std::stringstream ss;
    ss << std::hex << hexComponentCode;
    int value;
    ss >> value;

    return static_cast<float>(value);
}
