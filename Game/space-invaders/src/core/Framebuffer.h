#pragma once
#include "glm/vec4.hpp"

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, glm::vec4 clearColor = glm::vec4{0.f, 0.f, 0.f, 1.f});
    ~Framebuffer();

    void Bind() const;
    void BindAndClear() const;
    void Unbind() const;

    inline unsigned int GetColorBufferTexture0() const { return m_ColorBufferTexture0; }
    inline unsigned int GetColorBufferTexture1() const { return m_ColorBufferTexture1; }

private:
    
    unsigned int m_FBO;
    unsigned int m_ColorBufferTexture0;
    unsigned int m_ColorBufferTexture1;
    glm::vec4 m_ClearColor;
};
