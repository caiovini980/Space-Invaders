#include "UIManager.h"

#include "ResourceManager.h"
#include "Shader.h"

UIManager::UIManager(unsigned screenWidth, unsigned screenHeight)
    : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
    std::shared_ptr<Shader> defaultTextShader = ResourceManager::LoadShader("res/shaders/Text.vertex", "res/shaders/Text.frag", "DefaultText");
    m_TextRenderer = std::make_unique<TextRenderer>(screenWidth, screenHeight, "res/fonts/dogicapixel.ttf", 30, defaultTextShader);
}

void UIManager::RenderInGameScreen()
{
    m_TextRenderer->RenderText("LIVES: 999", 20.f, 20.f, 0.5f, glm::vec3{1.f});
}

void UIManager::RenderGameWinScreen()
{
    m_TextRenderer->RenderText("GOOD ENDING", 270, 200, 1.f, glm::vec3{1.f});
    m_TextRenderer->RenderText("For you, can't say the same for the aliens...", 210, 250, 0.4f, glm::vec3{1.f});
}
