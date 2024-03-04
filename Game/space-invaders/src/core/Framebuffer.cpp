#include "Framebuffer.h"

#include <iostream>

#include "Core.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
{
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    GLCall(glGenTextures(1, &m_ColorBufferTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorBufferTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferTexture, 0));

    GLCall(glGenRenderbuffers(1, &m_RBO));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height)));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    
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
    GLCall(glDeleteRenderbuffers(1, &m_RBO));
    GLCall(glDeleteTextures(1, &m_ColorBufferTexture));
}

void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
}

void Framebuffer::BindAndClear() const
{
    Bind();

    GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
