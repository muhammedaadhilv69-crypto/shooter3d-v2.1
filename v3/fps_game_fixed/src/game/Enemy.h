#ifndef ENEMY_H
#define ENEMY_H

#include "../engine/Mesh.h"
#include <glm/glm.hpp>

namespace Game {

class Enemy {
public:
    Enemy(const glm::vec3& position);
    ~Enemy();

    void update(float deltaTime, const glm::vec3& playerPos);
    void takeDamage(float damage);
    
    bool isAlive() const { return m_health > 0.0f; }
    glm::vec3 getPosition() const { return m_position; }
    Engine::Mesh& getMesh() { return m_mesh; }
    
    glm::mat4 getModelMatrix() const;
    glm::vec3 getColor() const;
    
    float getHealth() const { return m_health; }
    bool canSeePlayer(const glm::vec3& playerPos) const;

private:
    Engine::Mesh m_mesh;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    
    float m_health;
    float m_maxHealth;
    float m_speed;
    float m_detectionRange;
    
    bool m_active;
    float m_damageFlashTimer;
};

} // namespace Game

#endif // ENEMY_H
