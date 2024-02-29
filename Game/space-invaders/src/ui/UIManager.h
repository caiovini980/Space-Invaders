#pragma once
#include <memory>

#include "TextRenderer.h"

class UIManager
{
public:

    UIManager(unsigned int screenWidth, unsigned int screenHeight);

    void RenderInGameScreen();
    void RenderGameWinScreen();

private:

    unsigned int m_ScreenWidth{0};
    unsigned int m_ScreenHeight{0};
    std::unique_ptr<TextRenderer> m_TextRenderer;
    std::shared_ptr<Shader> m_DefaultTextShader;
    std::shared_ptr<Shader> m_BlinkTextShader;
};
