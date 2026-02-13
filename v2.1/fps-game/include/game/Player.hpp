#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "renderer/Camera.hpp"
#include "core/Input.hpp"

namespace fps::game {

class Weapon;
class GameWorld;

struct PlayerStats {
    float maxHealth = 100.0f;
    float health = 100.0f;
    float maxArmor = 100.0f;
    float armor = 0.0f;
    float speed = 5.0f;
    float sprintSpeed = 8.0f;
    float jumpForce = 8.0f;
    
    int kills = 0;
    int deaths = 0;
    int score = 0;
};

class Player {
public:
    Player();
    ~Player();
    
    bool Initialize(GameWorld* world);
    void Shutdown();
    
    void Update(float deltaTime);
    void Render();
    
    void TakeDamage(float damage);
    void Heal(float amount);
    void AddArmor(float amount);
    void AddScore(int points);
    
    void Respawn(const glm::vec3& position);
    bool IsAlive() const { return m_stats.health > 0.0f; }
    
    // Getters
    glm::vec3 GetPosition() const { return m_camera.GetPosition(); }
    glm::vec3 GetForward() const { return m_camera.GetForward(); }
    glm::vec3 GetRight() const { return m_camera.GetRight(); }
    renderer::Camera& GetCamera() { return m_camera; }
    const renderer::Camera& GetCamera() const { return m_camera; }
    const PlayerStats& GetStats() const { return m_stats; }
    PlayerStats& GetStats() { return m_stats; }
    
    // Weapon
    void EquipWeapon(std::shared_ptr<Weapon> weapon);
    std::shared_ptr<Weapon> GetCurrentWeapon() const { return m_currentWeapon; }
    
    // Input handling
    void HandleInput(float deltaTime);
    void OnMouseMove(float deltaX, float deltaY);
    
    // Movement
    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);
    void Jump();
    void Sprint(bool sprinting);
    void Crouch(bool crouching);
    
    // State
    bool IsSprinting() const { return m_sprinting; }
    bool IsCrouching() const { return m_crouching; }
    bool IsGrounded() const { return m_grounded; }
    bool IsAiming() const { return m_aiming; }
    
    void SetPosition(const glm::vec3& position);
    void SetRotation(float pitch, float yaw);
    
    // Collision
    void SetGrounded(bool grounded) { m_grounded = grounded; }
    float GetHeight() const { return m_crouching ? m_crouchHeight : m_standHeight; }
    float GetRadius() const { return m_radius; }
    
private:
    GameWorld* m_world;
    renderer::Camera m_camera;
    PlayerStats m_stats;
    
    // Movement
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    
    float m_pitch;
    float m_yaw;
    float m_targetPitch;
    float m_targetYaw;
    
    // Physics
    bool m_grounded;
    float m_verticalVelocity;
    
    // States
    bool m_sprinting;
    bool m_crouching;
    bool m_aiming;
    bool m_firing;
    
    // Dimensions
    float m_standHeight;
    float m_crouchHeight;
    float m_radius;
    
    // Weapon
    std::shared_ptr<Weapon> m_currentWeapon;
    
    // Input smoothing
    float m_mouseSensitivity;
    float m_movementSmoothing;
    glm::vec2 m_movementInput;
    glm::vec2 m_smoothedMovement;
    
    void UpdateMovement(float deltaTime);
    void UpdateCamera(float deltaTime);
    void UpdateWeapon(float deltaTime);
    void CheckGround();
    void ApplyGravity(float deltaTime);
};

} // namespace fps::game
