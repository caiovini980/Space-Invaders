#include "TextRenderer.h"

#include <ft2build.h>
#include <iostream>

#include FT_FREETYPE_H

#include "ResourceManager.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"

TextRenderer::TextRenderer(unsigned int width, unsigned int height, const std::string& fontPath, unsigned int fontSize, const std::shared_ptr<Shader>& shader)
    : m_Shader(shader), m_ScreenWidth(width), m_ScreenHeight(height)
{
    SetupShader(m_Shader);

    m_VAO = std::make_unique<VertexArray>();
        
    m_VBO = std::make_unique<VertexBuffer>(nullptr, 4 * 4 * sizeof(float), true);
    VertexBufferLayout layout{};
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    m_IBO = std::make_unique<IndexBuffer>(indices, 6);

    LoadFont(fontPath, fontSize);
}

TextRenderer::~TextRenderer()
{
    int totalTextures = static_cast<int>(m_Characters.size());

    std::vector<unsigned int> textureIds{};
    textureIds.reserve(totalTextures);

    for(const auto& character : m_Characters)
    {
        textureIds.push_back(character.second.TextureID);
    }

    GLCall(glDeleteTextures(totalTextures, textureIds.data()));
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color)
{
    m_VAO->Bind();

    m_Shader->Bind();
    m_Shader->SetUniform3f("u_TextColor", color);

    GLCall(glActiveTexture(GL_TEXTURE0));

    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++)
    {
        Character character = m_Characters[*c];

        float xPos = x + character.Bearing.x * scale;
        float yPos = y + (m_Characters['H'].Bearing.y - character.Bearing.y) * scale;

        float w = character.Size.x * scale;
        float h = character.Size.y * scale;

        float vertices[] =
        {
            // Pos                 UV
            xPos, yPos,         0.f, 0.f,
            xPos + w, yPos,     1.f, 0.f,
            xPos + w, yPos + h, 1.f, 1.f,
            xPos, yPos + h,     0.f, 1.f,
        };

        GLCall(glBindTexture(GL_TEXTURE_2D, character.TextureID));
        
        m_VBO->SetSubData(vertices, sizeof(vertices));

        GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));

        x += (character.Advance >> 6) * scale;
    }
}

void TextRenderer::SetupShader(const std::shared_ptr<Shader>& shader) const
{
    shader->Bind();

    glm::mat4 projection = glm::ortho(0.f, static_cast<float>(m_ScreenWidth), static_cast<float>(m_ScreenHeight), 0.f, -1.f, 1.f);
    shader->SetUniformMat4f("u_Projection", projection);
    shader->SetUniform1i("u_Text", 0);
}

void TextRenderer::ChangeShader(const std::shared_ptr<Shader>& shader)
{
    m_Shader = shader;
}

void TextRenderer::LoadFont(const std::string& fontPath, unsigned int fontSize)
{
    FT_Library freeType;

    if(FT_Init_FreeType(&freeType))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType library\n";
        return;
    }

    FT_Face face;
    if(FT_New_Face(freeType, fontPath.c_str(), 0, &face))
    {
        std::cout << "ERROR:FREETYPE: Could not load the font" << fontPath << "\n";
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    for(unsigned char c = 0; c < 128; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load glyph '" << c << "'\n";
            continue;
        }

        unsigned int textureId;
        GLCall(glGenTextures(1, &textureId));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
        GLCall(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        ));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        Character character{
            textureId,
            glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
            glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        m_Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(freeType);

    GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
}
