#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "math/Math.hpp"
#include "renderer/Mesh.hpp"

namespace fps::game {

class GameWorld;
class Player;
class Weapon;

enum class EnemyState {
    Idle,
    Patrol,
    Alert,
    Chase,
    Attack,
    Retreat,
    Dead
};

enum class EnemyType {
    Grunt,
    Soldier,
    Heavy,
    Sniper,
    Boss
};

struct EnemyStats {
    float maxHealth = 50.0f;
    float health = 50.0f;
    float speed = 3.0f;
    float runSpeed = 6.0f;
    float rotationSpeed = 3.0f;
    
    float attackRange = 20.0f;
    float optimalRange = 10.0f;
    float detectionRange = 30.0f;
    float fieldOfView = 120.0f; // Degrees
    
    float attackDamage = 10.0f;
    float attackCooldown = 1.0f;
    float accuracy = 0.7f;
    
    int scoreValue = 100;
};

class Enemy {
public:
    Enemy(EnemyType type);
    virtual ~Enemy();
    
    virtual bool Initialize(GameWorld* world);
    virtual void Shutdown();
    
    virtual void Update(float deltaTime);
    virtual void Render(const glm::mat4& viewProj);
    
    virtual void TakeDamage(float damage, const glm::vec3& hitDirection, Entity* attacker);
    virtual void OnDeath();
    
    void SetPosition(const glm::vec3& position);
    void SetRotation(float yaw);
    void SetTarget(Player* target);
    
    glm::vec3 GetPosition() const { return m_transform.position; }
    glm::vec3 GetForward() const { return m_transform.GetForward(); }
    float GetYaw() const { return m_yaw; }
    EnemyState GetState() const { return m_state; }
    EnemyType GetType() const { return m_type; }
    
    bool IsAlive() const { return m_stats.health > 0.0f; }
    bool IsDead() const { return m_state == EnemyState::Dead; }
    
    const EnemyStats& GetStats() const { return m_stats; }
    EnemyStats& GetStats() { return m_stats; }
    
    math::AABB GetBoundingBox() const;
    math::Sphere GetBoundingSphere() const;
    
    // AI
    bool CanSeeTarget() const;
    float GetDistanceToTarget() const;
    bool IsInAttackRange() const;
    
protected:
    EnemyType m_type;
    EnemyStats m_stats;
    EnemyState m_state;
    
    GameWorld* m_world;
    Player* m_target;
    
    math::Transform m_transform;
    float m_yaw;
    float m_pitch;
    
    // Movement
    glm::vec3 m_velocity;
    glm::vec3 m_targetPosition;
    std::vector<glm::vec3> m_patrolPoints;
    size_t m_currentPatrolIndex;
    
    // Timers
    float m_attackTimer;
    float m_stateTimer;
    float m_searchTimer;
    
    // Visuals
    std::unique_ptr<renderer::Mesh> m_mesh;
    // std::shared_ptr<renderer::Texture> m_texture;
    
    // Weapon
    std::shared_ptr<Weapon> m_weapon;
    
    // Animation
    float m_walkCycle;
    float m_animationSpeed;
    
    void UpdateAI(float deltaTime);
    void UpdateStateMachine(float deltaTime);
    void UpdateMovement(float deltaTime);
    void UpdateRotation(float deltaTime);
    
    // AI Behaviors
    void StateIdle(float deltaTime);
    void StatePatrol(float deltaTime);
    void StateAlert(float deltaTime);
    void StateChase(float deltaTime);
    void StateAttack(float deltaTime);
    void StateRetreat(float deltaTime);
    
    void TransitionToState(EnemyState newState);
    
    void FindNextPatrolPoint();
    void MoveTo(const glm::vec3& target, float speed, float deltaTime);
    void RotateTowards(const glm::vec3& target, float deltaTime);
    void Attack();
    void TryAttack();
    
    glm::vec3 GetRandomPointInRadius(float radius);
    bool HasLineOfSight(const glm::vec3& target) const;
    
    void UpdateAnimation(float deltaTime);
    void RenderHealthBar(const glm::mat4& viewProj);
};

class EnemyManager {
public:
    static EnemyManager& GetInstance();
    
    void Initialize(GameWorld* world);
    void Shutdown();
    
    void Update(float deltaTime);
    void Render(const glm::mat4& viewProj);
    
    Enemy* SpawnEnemy(EnemyType type, const glm::vec3& position);
    void DestroyEnemy(Enemy* enemy);
    void DestroyAllEnemies();
    
    void SetPlayerTarget(Player* player);
    
    size_t GetEnemyCount() const { return m_enemies.size(); }
    size_t GetAliveCount() const;
    
    std::vector<Enemy*> GetEnemiesInRadius(const glm::vec3& position, float radius);
    Enemy* GetClosestEnemy(const glm::vec3& position, float maxRange = FLT_MAX);
    
private:
    EnemyManager() = default;
    ~EnemyManager() = default;
    
    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;
    
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    GameWorld* m_world;
    Player* m_targetPlayer;
    bool m_initialized;
};

} // namespace fps::game
