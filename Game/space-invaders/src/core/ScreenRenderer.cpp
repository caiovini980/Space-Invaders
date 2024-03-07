#include "ScreenRenderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

ScreenRenderer::ScreenRenderer(const std::shared_ptr<Shader>& shader)
    : m_Shader(shader)
{
    m_VAO = std::make_unique<VertexArray>();
    
    float vertices[] =
    {
        // Pos in device coords (-1 to 1)     UV
        -1.f, -1.f,  0.f, 0.f,
         1.f, -1.f,  1.f, 0.f,
         1.f,  1.f,  1.f, 1.f,
        -1.f,  1.f,  0.f, 1.f
    };

    m_VBO = std::make_unique<VertexBuffer>(vertices, static_cast<unsigned int>(sizeof(vertices)));

    VertexBufferLayout layout{};
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    m_IBO = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader->Bind();
    m_Shader->SetUniform1i("u_ScreenTexture", 0);
}

ScreenRenderer::~ScreenRenderer()
{ }

void ScreenRenderer::Draw(unsigned int screenTexture)
{
    m_VAO->Bind();
    m_Shader->Bind();

    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, screenTexture));

    GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
}
