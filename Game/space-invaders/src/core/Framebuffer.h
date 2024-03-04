#pragma once

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    void Bind() const;
    void BindAndClear() const;
    void Unbind() const;

    inline unsigned int GetColorBufferTexture() const { return m_ColorBufferTexture; }

private:
    
    unsigned int m_FBO;
    unsigned int m_ColorBufferTexture;
    unsigned int m_RBO;
};
