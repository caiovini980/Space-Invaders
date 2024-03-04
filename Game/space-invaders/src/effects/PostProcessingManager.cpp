#include "PostProcessingManager.h"

#include "ResourceManager.h"

PostProcessingManager::PostProcessingManager()
{
    m_Shader = ResourceManager::LoadShader("res/shaders/PostProcessing.vertex", "res/shaders/PostProcessing.frag", "PostProcessing");
    m_ScreenRenderer = std::make_unique<ScreenRenderer>(m_Shader);
}

void PostProcessingManager::SetGrayscaleEnabled(bool bEnable)
{
    bIsGrayscaleEnabled = bEnable;

    m_Shader->Bind();
    m_Shader->SetUniform1i("u_GrayscaleEnabled", bEnable);
    m_Shader->Unbind();
}

void PostProcessingManager::RenderWithPostProcessing(unsigned screenTexture)
{
    m_ScreenRenderer->Draw(screenTexture);
}
