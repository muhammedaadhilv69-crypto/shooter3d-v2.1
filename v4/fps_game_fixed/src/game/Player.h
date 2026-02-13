#ifndef PLAYER_H
#define PLAYER_H

#include "../engine/Camera.h"
#include <glm/glm.hpp>

namespace Game {

class Player {
public:
    Player();
    ~Player();

    void update(float deltaTime);
    void processInput(int key, bool pressed);
    void processMouseMove(float xoffset, float yoffset);

    Engine::Camera& getCamera() { return m_camera; }
    glm::vec3 getPosition() const { return m_position; }
    float getHealth() const { return m_health; }
    
    void takeDamage(float damage);
    void heal(float amount);

    bool isAlive() const { return m_health > 0.0f; }

private:
    Engine::Camera m_camera;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    
    float m_health;
    float m_maxHealth;
    float m_speed;
    float m_jumpForce;
    
    bool m_keys[6]; // W, A, S, D, Space, Shift
    bool m_isGrounded;
};

} // namespace Game

#endif // PLAYER_H
