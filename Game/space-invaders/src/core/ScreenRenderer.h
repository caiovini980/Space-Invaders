#pragma once
#include <memory>

class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Shader;

class ScreenRenderer
{
public:
    ScreenRenderer(const std::shared_ptr<Shader>& shader);
    ~ScreenRenderer();

    void Draw(unsigned int screenTexture);

private:

    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
};
