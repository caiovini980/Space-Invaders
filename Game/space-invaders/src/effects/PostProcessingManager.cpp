#include "PostProcessingManager.h"

#include "Core.h"
#include "Framebuffer.h"
#include "ResourceManager.h"

PostProcessingManager::PostProcessingManager()
{
    m_Shader = ResourceManager::LoadShader("res/shaders/PostProcessing.vertex", "res/shaders/PostProcessing.frag", "PostProcessing");
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_VelocityTexture", 1);

    m_ScreenRenderer = std::make_unique<ScreenRenderer>(m_Shader);
}

void PostProcessingManager::SetGrayscaleEnabled(bool bEnable)
{
    bIsGrayscaleEnabled = bEnable;

    m_Shader->Bind();
    m_Shader->SetUniform1i("u_GrayscaleEnabled", bEnable);
    m_Shader->Unbind();
}

void PostProcessingManager::RenderWithPostProcessing(const Framebuffer& framebuffer)
{
    GLCall(glActiveTexture(GL_TEXTURE1));
    GLCall(glBindTexture(GL_TEXTURE_2D, framebuffer.GetColorBufferTexture1()));

    m_ScreenRenderer->Draw(framebuffer.GetColorBufferTexture0());
}
