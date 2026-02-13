#ifndef GAME_H
#define GAME_H

#include "../engine/Window.h"
#include "../engine/Renderer.h"
#include "Player.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Level.h"
#include "Particle.h"
#include <vector>
#include <memory>

namespace Game {

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void shutdown();

private:
    void processInput();
    void update(float deltaTime);
    void render();
    
    void handleKeyInput(int key, bool pressed);
    void handleMouseMove(double xoffset, double yoffset);
    void handleShooting();
    void checkCollisions();
    void spawnEnemies();

    Engine::Window m_window;
    Engine::Renderer m_renderer;
    
    Player m_player;
    std::unique_ptr<Weapon> m_weapon;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    Level m_level;
    ParticleSystem m_particles;
    
    bool m_initialized;
    bool m_shooting;
    
    float m_enemySpawnTimer;
    int m_score;
    int m_wave;
};

} // namespace Game

#endif // GAME_H
