#include "Player.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace Game {

Player::Player()
    : m_camera(glm::vec3(0.0f, 1.8f, 5.0f))
    , m_position(0.0f, 1.8f, 5.0f)
    , m_velocity(0.0f)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_speed(5.0f)
    , m_jumpForce(8.0f)
    , m_isGrounded(true)
{
    for (int i = 0; i < 6; ++i) m_keys[i] = false;
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    // Process movement
    glm::vec3 movement(0.0f);
    
    if (m_keys[0]) m_camera.processKeyboard(Engine::FORWARD, deltaTime); // W
    if (m_keys[1]) m_camera.processKeyboard(Engine::LEFT, deltaTime);    // A
    if (m_keys[2]) m_camera.processKeyboard(Engine::BACKWARD, deltaTime);// S
    if (m_keys[3]) m_camera.processKeyboard(Engine::RIGHT, deltaTime);   // D
    
    // Jumping
    if (m_keys[4] && m_isGrounded) { // Space
        m_velocity.y = m_jumpForce;
        m_isGrounded = false;
    }
    
    // Crouching
    if (m_keys[5]) { // Shift
        // Could implement crouching here
    }
    
    // Apply gravity
    if (!m_isGrounded) {
        m_velocity.y -= 20.0f * deltaTime;
    }
    
    // Update position
    m_position = m_camera.getPosition();
    m_position += m_velocity * deltaTime;
    
    // Ground check (simple)
    if (m_position.y <= 1.8f) {
        m_position.y = 1.8f;
        m_velocity.y = 0.0f;
        m_isGrounded = true;
    }
    
    m_camera.setPosition(m_position);
}

void Player::processInput(int key, bool pressed) {
    switch (key) {
        case SDL_SCANCODE_W: m_keys[0] = pressed; break;
        case SDL_SCANCODE_A: m_keys[1] = pressed; break;
        case SDL_SCANCODE_S: m_keys[2] = pressed; break;
        case SDL_SCANCODE_D: m_keys[3] = pressed; break;
        case SDL_SCANCODE_SPACE: m_keys[4] = pressed; break;
        case SDL_SCANCODE_LSHIFT: m_keys[5] = pressed; break;
    }
}

void Player::processMouseMove(float xoffset, float yoffset) {
    m_camera.processMouseMovement(xoffset, -yoffset);
}

void Player::takeDamage(float damage) {
    m_health = std::max(0.0f, m_health - damage);
}

void Player::heal(float amount) {
    m_health = std::min(m_maxHealth, m_health + amount);
}

} // namespace Game
