#pragma once
#include <memory>

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
    void ProcessInput(float deltaTime);
    void Render();
    void Close();

private:

    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
};
