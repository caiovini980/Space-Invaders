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

    std::shared_ptr<SpriteRenderer> m_SpriteRenderer;
    std::shared_ptr<Texture> m_Texture;
};
