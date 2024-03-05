#include "Framebuffer.h"

#include <iostream>

#include "Core.h"

Framebuffer::Framebuffer(unsigned width, unsigned height, glm::vec4 clearColor)
    : m_ClearColor(clearColor)
{
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    GLCall(glGenTextures(1, &m_ColorBufferTexture0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorBufferTexture0));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferTexture0, 0));

    GLCall(glGenTextures(1, &m_ColorBufferTexture1));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorBufferTexture1));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_ColorBufferTexture1, 0));

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    GLCall(glDrawBuffers(2, drawBuffers));
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR:FRAMEBUFFER: Framebuffer is not complete!\n";
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer()
{
    Unbind();

    GLCall(glDeleteFramebuffers(1, &m_FBO));
    GLCall(glDeleteTextures(1, &m_ColorBufferTexture0));
    GLCall(glDeleteTextures(1, &m_ColorBufferTexture1));
}

void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
}

void Framebuffer::BindAndClear() const
{
    Bind();

    GLCall(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
