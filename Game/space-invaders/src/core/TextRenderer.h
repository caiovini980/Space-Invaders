#pragma once
#include <map>
#include <memory>
#include <string>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class Texture;
class Shader;

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextRenderer
{
public:

    TextRenderer(unsigned int width, unsigned int height, const std::string& fontPath, unsigned int fontSize, const std::shared_ptr<Shader>& shader);
    ~TextRenderer();
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    
private:

    std::map<char, Character> m_Characters;
    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;

    void LoadFont(const std::string& fontPath, unsigned int fontSize);
};
