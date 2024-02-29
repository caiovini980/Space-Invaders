#include "UIManager.h"

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Shader.h"

UIManager::UIManager(unsigned screenWidth, unsigned screenHeight)
    : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
    std::shared_ptr<Shader> defaultTextShader = ResourceManager::LoadShader(
        "res/shaders/Text.vertex", "res/shaders/Text.frag", "DefaultText");
    
    m_TextRenderer = std::make_unique<TextRenderer>(screenWidth, screenHeight,
        "res/fonts/dogicapixel.ttf", 30, defaultTextShader);

    m_GameEndedGUIBackgroundSprite = ResourceManager::LoadTexture(
        "./res/textures/endgameGuiBackground.png", "EndgameGuiBackground", true);

    const glm::vec2 size = glm::vec2(m_ScreenWidth, m_ScreenHeight);
    constexpr glm::vec2 position = glm::vec2(0, 0);
    constexpr glm::vec3 color = glm::vec3(1.0);
    
    m_GameEndedGUIBackground = GameObject {
        position,
        size,
        m_GameEndedGUIBackgroundSprite,
        color
    };
}

void UIManager::RenderInGameScreen(unsigned int playerLives) const
{
    m_TextRenderer->RenderText("LIVES: " + std::to_string(playerLives), 20.f, 20.f, 1.f, glm::vec3{1.f});
}

void UIManager::RenderGameWinScreen() const
{
    m_TextRenderer->RenderText("GOOD ENDING", 270, 200, 1.f, glm::vec3{0.0f, 1.0f, 0.0f});
    m_TextRenderer->RenderText("For you, can't say the same for the aliens...", 210, 250, 0.4f, glm::vec3{1.f});
}

void UIManager::RenderGameOverScreen(const SpriteRenderer& renderer) const
{
    renderer.Draw( *m_GameEndedGUIBackgroundSprite, m_GameEndedGUIBackground.Position, m_GameEndedGUIBackground.Size);
    
    m_TextRenderer->RenderText("BAD ENDING", 270, 200, 1.f, glm::vec3{1.0f, 0.0f, 0.0f});
    m_TextRenderer->RenderText("There's nothing else you could do. The aliens are superior!", 150, 250, 0.4f, glm::vec3{1.f});
}
