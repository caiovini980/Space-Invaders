#include "Framebuffer.h"

#include <iostream>

#include "Core.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height, const std::vector<TextureDefinition>& additionalColorAttachments)
{
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    GLCall(glGenTextures(1, &m_MainColorBufferTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_MainColorBufferTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MainColorBufferTexture, 0));

    CreateAdditionalColorAttachments(width, height, additionalColorAttachments);
    EnableAdditionalColorAttachments(static_cast<unsigned int>(additionalColorAttachments.size()));
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR:FRAMEBUFFER: Framebuffer is not complete!\n";
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::CreateAdditionalColorAttachments(unsigned int width, unsigned int height, const std::vector<TextureDefinition>& additionalColorAttachments)
{
    if(additionalColorAttachments.empty())
    {
        return;        
    }

    m_AdditionalColorTextures.reserve(additionalColorAttachments.size());

    for(unsigned int i = 0; i < additionalColorAttachments.size(); i++)
    {
        m_AdditionalColorTextures.push_back(0);
        
        GLCall(glGenTextures(1, &m_AdditionalColorTextures[i]));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_AdditionalColorTextures[i]));
        
        const TextureDefinition& definition = additionalColorAttachments[i];

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, definition.InternalFormat, width, height, 0, definition.Format, definition.Type, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1 + i, GL_TEXTURE_2D, m_AdditionalColorTextures[i], 0));
    }
}

void Framebuffer::EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments)
{
    std::vector<GLenum> drawBuffers{};
    drawBuffers.reserve(1 + totalAdditionalColorAttachments);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);

    for(unsigned int i = 0; i < totalAdditionalColorAttachments; i++)
    {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT1 + i);
    }
    
    GLCall(glDrawBuffers(1 + totalAdditionalColorAttachments, drawBuffers.data()));
}

Framebuffer::~Framebuffer()
{
    Unbind();

    GLCall(glDeleteFramebuffers(1, &m_FBO));
    GLCall(glDeleteTextures(1, &m_MainColorBufferTexture));

    if(!m_AdditionalColorTextures.empty())
    {
        GLCall(glDeleteTextures(static_cast<int>(m_AdditionalColorTextures.size()), m_AdditionalColorTextures.data()));
    }
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

unsigned Framebuffer::GetAdditionalColorTexture(unsigned int index) const
{
    if(index >= static_cast<unsigned int>(m_AdditionalColorTextures.size()))
    {
        std::cout << "ERROR:FRAMEBUFFER Trying to get an invalid additional color texture with index " << index << "\n";
        return 0;
    }

    return m_AdditionalColorTextures[index];
}
