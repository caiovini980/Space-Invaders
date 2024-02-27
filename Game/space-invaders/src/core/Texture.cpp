#include "Texture.h"

#include "Core.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath, bool bUseAlpha, bool bFlipVertically)
    : m_FilePath(filePath)
{
    if(bFlipVertically)
    {
        stbi_set_flip_vertically_on_load(1);
    }

    unsigned int internalFormat = bUseAlpha ? GL_RGBA8 : GL_RGB;
    unsigned int format = bUseAlpha ? GL_RGBA : GL_RGB;
    int desiredChannels = bUseAlpha ? 4 : 3;

    m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, desiredChannels);
    
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
