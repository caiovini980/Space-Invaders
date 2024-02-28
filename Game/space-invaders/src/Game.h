#pragma once
#include <memory>

#include "Input.h"
#include "PlayerManager.h"

class Texture;
class SpriteRenderer;
class Shader;

class Game
{
public:
    Game();
    ~Game();
    
    void Init();
    void Update(float deltaTime);
    void ProcessInput(float deltaTime, const Input& input);
    void Render();
    void Close();

private:
    const float WIDTH = 800.0f;
    const float HEIGHT = 600.0f;
    
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
    std::unique_ptr<PlayerManager> m_PlayerManager;
};
