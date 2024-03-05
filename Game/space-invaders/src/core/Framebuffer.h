#pragma once
#include <vector>

#include "Texture.h"
#include "glm/vec4.hpp"

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, const std::vector<TextureDefinition>& additionalColorAttachments);
    ~Framebuffer();

    void Bind() const;
    void BindAndClear() const;
    void Unbind() const;
    unsigned int GetAdditionalColorTexture(unsigned int index) const;

    inline unsigned int GetMainColorBufferTexture() const { return m_MainColorBufferTexture; }

private:
    
    unsigned int m_FBO;
    unsigned int m_MainColorBufferTexture;
    std::vector<unsigned int> m_AdditionalColorTextures{};
    glm::vec4 m_ClearColor{0.f, 0.f, 0.f, 1.f};

    void CreateAdditionalColorAttachments(unsigned int width, unsigned int height, const std::vector<TextureDefinition>& additionalColorAttachments);
    void EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments);
};
