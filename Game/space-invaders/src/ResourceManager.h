#pragma once
#include <map>
#include <memory>
#include <string>

#include "Shader.h"
#include "Texture.h"


class ResourceManager
{
public:

    static std::shared_ptr<Shader> LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name);
    static std::shared_ptr<Shader> GetShader(const std::string& name);
    static std::shared_ptr<Texture> LoadTexture(const std::string& texturePath, const std::string& name, bool bUseAlpha = true, bool bFlipVertically = false);
    static std::shared_ptr<Texture> GetTexture(const std::string& name);
    static void ClearAll();

private:
    
    static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
    static std::map<std::string, std::shared_ptr<Texture>> m_Textures;

    ResourceManager() = default;

    static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragShaderPath);
    static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& texturePath,bool bUseAlpha = true, bool bFlipVertically = false);
};
