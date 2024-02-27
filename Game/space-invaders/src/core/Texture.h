#pragma once

#include <string>

class Texture
{
public:

    Texture(const std::string& filePath, bool bUseAlpha = true, bool bFlipVertically = true);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:

    unsigned int m_RendererID{0};
    std::string m_FilePath{};
    unsigned char* m_LocalBuffer{nullptr};
    int m_Width{0};
    int m_Height{0};
    int m_BPP{0};
};
