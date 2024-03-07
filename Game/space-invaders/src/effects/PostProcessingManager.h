#pragma once
#include <memory>

class Framebuffer;
class Shader;
#include "ScreenRenderer.h"

class PostProcessingManager
{
public:

    PostProcessingManager();

    void SetGrayscaleEnabled(bool bEnable);
    void RenderWithPostProcessing(const Framebuffer& framebuffer);

    inline bool IsGrayscaleEnabled() const { return bIsGrayscaleEnabled; }

private:

    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<ScreenRenderer> m_ScreenRenderer;
    bool bIsGrayscaleEnabled{false};
};
