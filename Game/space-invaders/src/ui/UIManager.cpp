#include "UIManager.h"

#include <GLFW/glfw3.h>

#include "Input.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Shader.h"
#include "utils/GameTime.h"

UIManager::UIManager(unsigned screenWidth, unsigned screenHeight)
    : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
    m_DefaultTextShader = ResourceManager::LoadShader("res/shaders/Text.vertex", "res/shaders/Text.frag", "DefaultText");
    m_BlinkTextShader = ResourceManager::LoadShader("res/shaders/Text.vertex", "res/shaders/TextBlink.frag", "BlinkText");
    
    m_MainMenuTextRenderer = std::make_unique<TextRenderer>(m_ScreenWidth, m_ScreenHeight, "res/fonts/dogicapixelbold.ttf", 50, m_DefaultTextShader);
    m_TextRenderer = std::make_unique<TextRenderer>(m_ScreenWidth, m_ScreenHeight, "res/fonts/dogicapixel.ttf", 30, m_DefaultTextShader);
    m_TextRenderer->SetupShader(m_BlinkTextShader);

    m_BlinkTextShader->Bind();
    m_BlinkTextShader->SetUniform1f("u_Speed", 1.5f);

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

void UIManager::ProcessMainMenuInput(const Input& input)
{
    if(input.GetKey(GLFW_KEY_ENTER))
    {
        bIsPlayRequested = true;
    }
}

void UIManager::RenderMainMenuScreen() const
{
    m_MainMenuTextRenderer->RenderText("SPACE", 275, 170, 1.f, glm::vec3{1.0f, 0.0f, 1.0f});
    m_MainMenuTextRenderer->RenderText("INVADERS", 202, 230, 1.f, glm::vec3{0.0f, 1.0f, 0.0f});

    m_BlinkTextShader->Bind();
    m_BlinkTextShader->SetUniform1f("u_Time", GameTime::Time);
    m_TextRenderer->ChangeShader(m_BlinkTextShader);
    m_TextRenderer->RenderText("Press ENTER", 331, 400, 0.5f, glm::vec3{1.f});
    
    m_TextRenderer->ChangeShader(m_DefaultTextShader);
    m_TextRenderer->RenderText("Controls", 20, 520, 0.4f, glm::vec3{0.65f});
    m_TextRenderer->RenderText("A and D to move", 20, 540, 0.4f, glm::vec3{1.f});
    m_TextRenderer->RenderText("SPACE to shoot", 20, 560, 0.4f, glm::vec3{1.f});
}

void UIManager::ExitMainMenu()
{
    bIsPlayRequested = false;

    m_MainMenuTextRenderer.reset();
    m_TextRenderer->ChangeShader(m_DefaultTextShader);
}

void UIManager::RenderInGameScreen(unsigned int playerLives) const
{
    m_TextRenderer->RenderText("LIVES: " + std::to_string(playerLives), 20.f, 20.f, 1.f, glm::vec3{1.f});
}

void UIManager::RenderGameWinScreen() const
{
    m_TextRenderer->ChangeShader(m_DefaultTextShader);
    m_TextRenderer->RenderText("GOOD ENDING", 270, 200, 1.f, glm::vec3{0.0f, 1.0f, 0.0f});
    m_TextRenderer->RenderText("For you, can't say the same for the aliens...", 210, 250, 0.4f, glm::vec3{1.f});

    RenderRestartMessage();
}

void UIManager::RenderGameOverScreen(const SpriteRenderer& renderer) const
{
    renderer.Draw( *m_GameEndedGUIBackgroundSprite, m_GameEndedGUIBackground.Position, m_GameEndedGUIBackground.Size);

    m_TextRenderer->ChangeShader(m_DefaultTextShader);
    m_TextRenderer->RenderText("BAD ENDING", 270, 200, 1.f, glm::vec3{1.0f, 0.0f, 0.0f});
    m_TextRenderer->RenderText("There's nothing else you could do. The aliens are superior!", 150, 250, 0.4f, glm::vec3{1.f});

    RenderRestartMessage();
}

void UIManager::RenderRestartMessage() const
{
    m_BlinkTextShader->Bind();
    m_BlinkTextShader->SetUniform1f("u_Time", GameTime::Time);
    m_TextRenderer->ChangeShader(m_BlinkTextShader);
    m_TextRenderer->RenderText("Press R to restart", 20.f, 20.f, 0.5f, glm::vec3{1.f});
}

void UIManager::Restart() const
{
    m_TextRenderer->ChangeShader(m_DefaultTextShader);
}
