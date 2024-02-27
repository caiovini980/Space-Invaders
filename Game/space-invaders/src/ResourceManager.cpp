#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <memory>
#include <sstream>

#include "stb_image/stb_image.h"

std::map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders{};
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures{};

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name)
{
    m_Shaders[name] = LoadShaderFromFile(vertexShaderPath, fragShaderPath);

    return m_Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return m_Shaders[name];
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& texturePath, const std::string& name, bool bUseAlpha, bool bFlipVertically)
{
    m_Textures[name] = LoadTextureFromFile(texturePath, bUseAlpha, bFlipVertically);

    return m_Textures[name];
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
    return m_Textures[name];
}

void ResourceManager::ClearAll()
{
    m_Shaders.clear();
    m_Textures.clear();
}

std::shared_ptr<Shader> ResourceManager::LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragShaderPath)
{
    std::string vertexSourceCode{};
    std::string fragSourceCode{};

    try
    {
        std::ifstream vertexShaderFile{vertexShaderPath};
        std::ifstream fragShaderFile{fragShaderPath};

        std::stringstream vertexStream;
        std::stringstream fragStream;

        vertexStream << vertexShaderFile.rdbuf();
        fragStream << fragShaderFile.rdbuf();

        vertexShaderFile.close();
        fragShaderFile.close();

        vertexSourceCode = vertexStream.str();
        fragSourceCode = fragStream.str();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << vertexShaderPath << " | " << fragShaderPath;
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexSourceCode, fragSourceCode);

    return shader;
}

std::shared_ptr<Texture> ResourceManager::LoadTextureFromFile(const std::string& texturePath, bool bUseAlpha, bool bFlipVertically)
{
    stbi_set_flip_vertically_on_load(bFlipVertically);

    int desiredChannels = bUseAlpha ? 4 : 3;

    int width;
    int height;
    int channels;

    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, desiredChannels);

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(data, width, height, bUseAlpha);
    
    if(data)
    {
        stbi_image_free(data);
    }

    return texture;
}
