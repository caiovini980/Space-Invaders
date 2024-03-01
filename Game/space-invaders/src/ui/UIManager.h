#pragma once
#include <memory>

#include "GameObject.h"
#include "TextRenderer.h"
#include "Texture.h"

class SpriteRenderer;

class UIManager
{
public:

    UIManager(unsigned int screenWidth, unsigned int screenHeight);

    void RenderInGameScreen(unsigned int playerLives) const;
    void RenderGameWinScreen() const;
    void RenderGameOverScreen(const SpriteRenderer& renderer) const;
    void Restart() const;

private:

    unsigned int m_ScreenWidth{0};
    unsigned int m_ScreenHeight{0};
    std::unique_ptr<TextRenderer> m_TextRenderer;
    std::shared_ptr<Shader> m_DefaultTextShader;
    std::shared_ptr<Shader> m_BlinkTextShader;

    GameObject m_GameEndedGUIBackground;
    std::shared_ptr<Texture> m_GameEndedGUIBackgroundSprite;

    void RenderRestartMessage() const;
};
