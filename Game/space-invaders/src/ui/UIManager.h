#pragma once
#include <memory>

#include "GameObject.h"
#include "TextRenderer.h"
#include "Texture.h"

class Input;
class SpriteRenderer;

class UIManager
{
public:

    UIManager(unsigned int screenWidth, unsigned int screenHeight);

    void ProcessMainMenuInput(const Input& input);
    void RenderMainMenuScreen() const;
    void ExitMainMenu();
    void RenderInGameScreen(unsigned int playerLives) const;
    void RenderGameWinScreen() const;
    void RenderGameOverScreen(const SpriteRenderer& renderer) const;
    void Restart() const;

    inline bool IsPlayRequested() const { return bIsPlayRequested; }

private:

    unsigned int m_ScreenWidth{0};
    unsigned int m_ScreenHeight{0};
    std::unique_ptr<TextRenderer> m_TextRenderer;
    std::unique_ptr<TextRenderer> m_MainMenuTextRenderer;
    std::shared_ptr<Shader> m_DefaultTextShader;
    std::shared_ptr<Shader> m_BlinkTextShader;
    bool bIsPlayRequested{false};

    GameObject m_GameEndedGUIBackground;
    std::shared_ptr<Texture> m_GameEndedGUIBackgroundSprite;

    void RenderRestartMessage() const;
};
