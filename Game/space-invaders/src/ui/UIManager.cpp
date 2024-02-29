#include "UIManager.h"

#include "ResourceManager.h"
#include "Shader.h"

UIManager::UIManager(unsigned screenWidth, unsigned screenHeight)
    : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
    std::shared_ptr<Shader> defaultTextShader = ResourceManager::LoadShader("res/shaders/Text.vertex", "res/shaders/Text.frag", "DefaultText");
    m_TextRenderer = std::make_unique<TextRenderer>(screenWidth, screenHeight, "res/fonts/dogicapixel.ttf", 15, defaultTextShader);
}

void UIManager::RenderInGameScreen()
{
    m_TextRenderer->RenderText("LIVES: 999", 20.f, 20.f, 1.f, glm::vec3{1.f});
}
