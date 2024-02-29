#pragma once
#include <memory>

#include "TextRenderer.h"

class UIManager
{
public:

    UIManager(unsigned int screenWidth, unsigned int screenHeight);

    void RenderInGameScreen();

private:

    unsigned int m_ScreenWidth{0};
    unsigned int m_ScreenHeight{0};
    std::unique_ptr<TextRenderer> m_TextRenderer;
};
