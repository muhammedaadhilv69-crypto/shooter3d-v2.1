#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "renderer/Renderer.hpp"
#include "renderer/Camera.hpp"

namespace fps::game {

class Player;
class Enemy;
class Weapon;
class Projectile;
class Level;

struct GameConfig {
    int targetFPS = 144;
    bool vsync = true;
    bool fullscreen = false;
    int windowWidth = 1280;
    int windowHeight = 720;
    float mouseSensitivity = 0.1f;
    float masterVolume = 1.0f;
    float sfxVolume = 1.0f;
    float musicVolume = 0.5f;
};

struct GameState {
    bool isPlaying = false;
    bool isPaused = false;
    float gameTime = 0.0f;
    int waveNumber = 0;
    int enemiesKilled = 0;
    int totalScore = 0;
    
    void Reset() {
        isPlaying = false;
        isPaused = false;
        gameTime = 0.0f;
        waveNumber = 0;
        enemiesKilled = 0;
        totalScore = 0;
    }
};

class GameWorld {
public:
    GameWorld();
    ~GameWorld();
    
    bool Initialize(const GameConfig& config);
    void Shutdown();
    
    void Update(float deltaTime);
    void Render();
    
    void StartGame();
    void PauseGame();
    void ResumeGame();
    void EndGame();
    void RestartGame();
    
    // Spawning
    void SpawnPlayer(const glm::vec3& position);
    Enemy* SpawnEnemy(const glm::vec3& position);
    void SpawnWeaponPickup(const glm::vec3& position, std::shared_ptr<Weapon> weapon);
    void SpawnAmmoPickup(const glm::vec3& position, int ammoAmount);
    void SpawnHealthPickup(const glm::vec3& position, float healthAmount);
    
    // Wave system
    void StartNextWave();
    void EndWave();
    int CalculateEnemiesForWave(int wave);
    
    // Queries
    Player* GetPlayer() const { return m_player.get(); }
    Level* GetLevel() const { return m_level.get(); }
    const GameState& GetGameState() const { return m_gameState; }
    GameState& GetGameState() { return m_gameState; }
    const GameConfig& GetConfig() const { return m_config; }
    
    bool IsPlaying() const { return m_gameState.isPlaying; }
    bool IsPaused() const { return m_gameState.isPaused; }
    
    // Collision queries
    bool Raycast(const math::Ray& ray, float maxDistance, math::RaycastHit& outHit);
    bool Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, math::RaycastHit& outHit);
    std::vector<Enemy*> GetEnemiesInRadius(const glm::vec3& position, float radius);
    
    // Audio
    void PlaySound(const std::string& soundName, const glm::vec3& position = glm::vec3(0.0f));
    void PlayMusic(const std::string& musicName);
    void StopMusic();
    
    // Effects
    void SpawnExplosion(const glm::vec3& position, float radius, float damage);
    void SpawnImpactEffect(const glm::vec3& position, const glm::vec3& normal);
    void SpawnMuzzleFlash(const glm::vec3& position, const glm::vec3& direction);
    void SpawnBloodEffect(const glm::vec3& position, const glm::vec3& direction);
    
private:
    GameConfig m_config;
    GameState m_gameState;
    
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Level> m_level;
    
    // Managers are singletons, but we keep references for convenience
    // EnemyManager, ProjectileManager, etc.
    
    // Wave system
    float m_waveTimer;
    float m_spawnTimer;
    int m_enemiesToSpawn;
    std::vector<glm::vec3> m_spawnPoints;
    
    void UpdateWaveSystem(float deltaTime);
    void SpawnWaveEnemies();
    glm::vec3 GetRandomSpawnPoint();
    
    void InitializeSpawnPoints();
    void CheckWinCondition();
    void CheckLoseCondition();
    
    void RenderWorld();
    void RenderUI();
    void RenderDebug();
};

// Global game instance
GameWorld* GetGameInstance();
void SetGameInstance(GameWorld* instance);

} // namespace fps::game
