#pragma once
#include <memory>
#include <vector>

#include "EGameState.h"
#include "Input.h"
#include "interfaces/IProjectileHandler.h"

class PostProcessingManager;
class Framebuffer;
class UIManager;
class TextRenderer;
class Texture;
class SpriteRenderer;
class Shader;
class GameLevel;
class PlayerManager;
class BackgroundManager;

class Game : public IProjectileHandler
{
public:
    Game();
    ~Game();
    
    void Init();
    bool HasGameEnded();
    void HandleGameLost();
    void Update(float deltaTime);
    void UpdatePlayerProjectiles(float deltaTime);
    void UpdateEnemyProjectiles(float deltaTime);
    void ProcessInput(float deltaTime, const Input& input);
    void RenderProjectiles() const;
    void Render();
    void Close();

    void AddEnemyProjectile(GameObject&& projectile) override;
    void AddPlayerProjectile(GameObject&& projectile) override;
    void CheckEnemyCollisions(GameObject& projectile) const;

private:
    const float WIDTH = 800.0f;
    const float HEIGHT = 600.0f;

    std::vector<GameObject> m_EnemyProjectiles;
    std::vector<GameObject> m_PlayerProjectiles;
    
    std::unique_ptr<PlayerManager> m_PlayerManager;
    std::unique_ptr<BackgroundManager> m_BackgroundManager;
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
    std::unique_ptr<GameLevel> m_Level;
    std::unique_ptr<UIManager> m_UIManager;
    std::unique_ptr<PostProcessingManager> m_PostProcessingManager;
    std::unique_ptr<Framebuffer> m_Framebuffer;
    
    EGameState m_CurrentState{EGameState::MainMenu};

    bool TryCollidingWithBarriers(GameObject& projectile) const;
    void StartGame();
    void HandleGameWon();
    void Restart();
    void RemoveDestroyedProjectiles();
};
