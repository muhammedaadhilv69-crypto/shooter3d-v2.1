#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Game {

Game::Game()
    : m_window("FPS Game - WASD Move, Mouse Look, LMB Shoot, R Reload", 1280, 720)
    , m_initialized(false)
    , m_shooting(false)
    , m_enemySpawnTimer(0.0f)
    , m_score(0)
    , m_wave(1)
{
}

Game::~Game() {
    shutdown();
}

bool Game::init() {
    std::cout << "=== FPS Game ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  Left Click - Shoot" << std::endl;
    std::cout << "  R - Reload" << std::endl;
    std::cout << "  ESC - Quit" << std::endl;
    std::cout << "================" << std::endl;
    
    if (!m_window.init()) {
        std::cerr << "Failed to initialize window!" << std::endl;
        return false;
    }
    
    if (!m_renderer.init()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return false;
    }
    
    m_renderer.setViewport(1280, 720);
    m_renderer.setCamera(&m_player.getCamera());
    
    // Setup input callbacks
    m_window.setKeyCallback([this](int key, bool pressed) {
        handleKeyInput(key, pressed);
    });
    
    m_window.setMouseCallback([this](double x, double y) {
        handleMouseMove(x, y);
    });
    
    m_window.captureMouse(true);
    
    // Initialize game objects
    m_weapon = std::make_unique<Weapon>(WeaponType::RIFLE);
    m_level.generate();
    
    // Spawn initial enemies
    spawnEnemies();
    
    m_initialized = true;
    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "Wave " << m_wave << " - Fight!" << std::endl;
    
    return true;
}

void Game::run() {
    if (!m_initialized) return;
    
    while (m_window.isRunning()) {
        m_window.processEvents();
        
        float deltaTime = m_window.getDeltaTime();
        deltaTime = std::min(deltaTime, 0.1f); // Cap delta time
        
        update(deltaTime);
        render();
        
        m_window.swap();
    }
}

void Game::update(float deltaTime) {
    // Update player
    m_player.update(deltaTime);
    
    // Update weapon
    if (m_weapon) {
        m_weapon->update(deltaTime);
        
        if (m_shooting && m_weapon->canFire()) {
            handleShooting();
        }
    }
    
    // Update enemies
    glm::vec3 playerPos = m_player.getPosition();
    for (auto& enemy : m_enemies) {
        enemy->update(deltaTime, playerPos);
    }
    
    // Remove dead enemies
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [this](const std::unique_ptr<Enemy>& e) {
                if (!e->isAlive()) {
                    m_score += 100;
                    std::cout << "Enemy destroyed! Score: " << m_score << std::endl;
                    
                    // Spawn particles
                    m_particles.emit(e->getPosition(), glm::vec3(0, 1, 0),
                                   glm::vec3(1.0f, 0.5f, 0.0f), 20);
                    return true;
                }
                return false;
            }),
        m_enemies.end()
    );
    
    // Update particles
    m_particles.update(deltaTime);
    
    // Spawn new enemies
    m_enemySpawnTimer += deltaTime;
    if (m_enemies.empty() && m_enemySpawnTimer > 3.0f) {
        m_wave++;
        std::cout << "\n=== Wave " << m_wave << " ===" << std::endl;
        spawnEnemies();
        m_enemySpawnTimer = 0.0f;
    }
    
    // Check collisions
    checkCollisions();
    
    // Game over check
    if (!m_player.isAlive()) {
        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Final Score: " << m_score << std::endl;
        std::cout << "Waves Survived: " << m_wave - 1 << std::endl;
        m_window.shutdown();
    }
}

void Game::render() {
    m_renderer.clear(glm::vec3(0.2f, 0.3f, 0.4f));
    
    // Render level
    m_level.render(m_renderer);
    
    // Render enemies
    for (auto& enemy : m_enemies) {
        if (enemy->isAlive()) {
            m_renderer.renderMesh(enemy->getMesh(), enemy->getModelMatrix(), 
                                enemy->getColor());
        }
    }
    
    // Render particles
    m_particles.render(m_renderer);
    
    // Render weapon (in front of camera)
    if (m_weapon) {
        glm::vec3 camPos = m_player.getCamera().getPosition();
        glm::vec3 camFront = m_player.getCamera().getFront();
        glm::vec3 camRight = m_player.getCamera().getRight();
        
        glm::vec3 weaponPos = camPos + camFront * 0.5f + camRight * 0.3f - 
                             glm::vec3(0, 0.3f, 0);
        
        glm::mat4 weaponModel = glm::mat4(1.0f);
        weaponModel = glm::translate(weaponModel, weaponPos);
        weaponModel = glm::scale(weaponModel, glm::vec3(0.1f, 0.05f, 0.3f));
        
        m_renderer.renderMesh(m_weapon->getMesh(), weaponModel, 
                            glm::vec3(0.2f, 0.2f, 0.2f));
    }
}

void Game::handleKeyInput(int key, bool pressed) {
    m_player.processInput(key, pressed);
    
    if (pressed) {
        if (key == SDL_SCANCODE_R && m_weapon) {
            m_weapon->reload();
        }
    }
    
    // Handle mouse button (keycode 1000 = left mouse button)
    if (key == 1000) {
        m_shooting = pressed;
    }
}

void Game::handleMouseMove(double xoffset, double yoffset) {
    m_player.processMouseMove(xoffset, yoffset);
}

void Game::handleShooting() {
    m_weapon->fire();
    
    // Raycast to check hits
    glm::vec3 origin = m_player.getCamera().getPosition();
    glm::vec3 direction = m_player.getCamera().getFront();
    
    // Check enemy hits (simple distance check for now)
    for (auto& enemy : m_enemies) {
        if (!enemy->isAlive()) continue;
        
        glm::vec3 toEnemy = enemy->getPosition() - origin;
        float distance = glm::length(toEnemy);
        
        if (distance < 50.0f) {
            glm::vec3 toEnemyDir = glm::normalize(toEnemy);
            float dot = glm::dot(direction, toEnemyDir);
            
            // Check if aiming at enemy (within a cone)
            if (dot > 0.98f) {
                enemy->takeDamage(25.0f);
                
                // Hit particles
                m_particles.emit(enemy->getPosition(), -direction,
                               glm::vec3(1.0f, 0.0f, 0.0f), 5);
                
                if (!enemy->isAlive()) {
                    std::cout << "Enemy killed!" << std::endl;
                }
            }
        }
    }
}

void Game::checkCollisions() {
    // Simple enemy damage
    glm::vec3 playerPos = m_player.getPosition();
    
    for (auto& enemy : m_enemies) {
        if (!enemy->isAlive()) continue;
        
        float distance = glm::length(enemy->getPosition() - playerPos);
        if (distance < 2.0f) {
            m_player.takeDamage(5.0f * m_window.getDeltaTime());
        }
    }
}

void Game::spawnEnemies() {
    int enemyCount = 3 + m_wave;
    
    for (int i = 0; i < enemyCount; ++i) {
        float angle = (float)i / enemyCount * 2.0f * M_PI;
        float radius = 10.0f + (m_wave * 2.0f);
        
        glm::vec3 pos(
            cos(angle) * radius,
            0.5f,
            sin(angle) * radius
        );
        
        m_enemies.push_back(std::make_unique<Enemy>(pos));
    }
    
    std::cout << "Spawned " << enemyCount << " enemies!" << std::endl;
}

void Game::shutdown() {
    m_enemies.clear();
    m_weapon.reset();
    m_window.shutdown();
}

} // namespace Game
