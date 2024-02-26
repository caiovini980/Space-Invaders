#pragma once

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
};
