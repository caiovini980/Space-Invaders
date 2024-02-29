#pragma once
#include <memory>
#include <vector>

#include "Input.h"
#include "interfaces/IProjectileHandler.h"
#include "players/PlayerManager.h"

class UIManager;
class TextRenderer;
class Texture;
class SpriteRenderer;
class Shader;
class GameLevel;

class Game : public IProjectileHandler
{
public:
    Game();
    ~Game();
    
    void Init();
    void Update(float deltaTime);
    void UpdatePlayerProjectiles(float deltaTime);
    void UpdateEnemyProjectiles(float deltaTime);
    void ProcessInput(float deltaTime, const Input& input);
    void RenderProjectiles() const;
    void Render();
    void Close();

    void AddEnemyProjectile(GameObject&& projectile) override;
    void AddPlayerProjectile(GameObject&& projectile) override;

private:
    const float WIDTH = 800.0f;
    const float HEIGHT = 600.0f;

    std::vector<GameObject> m_EnemyProjectiles;
    std::vector<GameObject> m_PlayerProjectiles;
    
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
    std::unique_ptr<GameLevel> m_Level;
    std::unique_ptr<PlayerManager> m_PlayerManager;
    std::unique_ptr<UIManager> m_UIManager;
    
    void RemoveDestroyedProjectiles();
};
