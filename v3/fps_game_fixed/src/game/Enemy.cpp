#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Game {

Enemy::Enemy(const glm::vec3& position)
    : m_mesh(Engine::Mesh::createCube(glm::vec3(1.0f, 0.2f, 0.2f)))
    , m_position(position)
    , m_velocity(0.0f)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_speed(2.0f)
    , m_detectionRange(15.0f)
    , m_active(false)
    , m_damageFlashTimer(0.0f)
{
}

Enemy::~Enemy() {
}

void Enemy::update(float deltaTime, const glm::vec3& playerPos) {
    if (!isAlive()) return;
    
    // Update damage flash
    if (m_damageFlashTimer > 0.0f) {
        m_damageFlashTimer -= deltaTime;
    }
    
    // Check if player is in range
    float distance = glm::length(playerPos - m_position);
    
    if (distance < m_detectionRange) {
        m_active = true;
        
        // Move towards player
        glm::vec3 direction = glm::normalize(playerPos - m_position);
        direction.y = 0.0f; // Keep on ground plane
        
        if (distance > 2.0f) { // Stop at close range
            m_velocity = direction * m_speed;
        } else {
            m_velocity = glm::vec3(0.0f);
        }
    } else {
        m_active = false;
        m_velocity *= 0.95f; // Slow down
    }
    
    // Apply gravity
    m_velocity.y -= 20.0f * deltaTime;
    
    // Update position
    m_position += m_velocity * deltaTime;
    
    // Ground collision
    if (m_position.y < 0.5f) {
        m_position.y = 0.5f;
        m_velocity.y = 0.0f;
    }
}

void Enemy::takeDamage(float damage) {
    m_health -= damage;
    m_damageFlashTimer = 0.2f;
    m_active = true; // Activate on damage
}

bool Enemy::canSeePlayer(const glm::vec3& playerPos) const {
    float distance = glm::length(playerPos - m_position);
    return distance < m_detectionRange;
}

glm::mat4 Enemy::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, glm::vec3(1.0f));
    return model;
}

glm::vec3 Enemy::getColor() const {
    if (m_damageFlashTimer > 0.0f) {
        return glm::vec3(1.0f, 1.0f, 1.0f); // White flash when damaged
    }
    
    if (!isAlive()) {
        return glm::vec3(0.3f, 0.3f, 0.3f); // Gray when dead
    }
    
    if (m_active) {
        return glm::vec3(1.0f, 0.2f, 0.2f); // Red when active
    }
    
    return glm::vec3(0.8f, 0.4f, 0.1f); // Orange when idle
}

} // namespace Game
