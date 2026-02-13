#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "math/Math.hpp"

namespace fps::game {

class GameWorld;
class Entity;

enum class ProjectileType {
    Bullet,
    Rocket,
    Grenade,
    Plasma,
    Laser
};

struct ProjectileData {
    ProjectileType type = ProjectileType::Bullet;
    float damage = 25.0f;
    float speed = 100.0f;
    float lifetime = 5.0f;
    float gravity = 0.0f;
    float radius = 0.05f;
    
    // Explosion (for rockets/grenades)
    float explosionRadius = 0.0f;
    float explosionDamage = 0.0f;
    float explosionForce = 0.0f;
    
    // Penetration
    float penetrationPower = 0.0f; // Damage retained after penetration
    int maxPenetrations = 0;
    
    // Visual
    glm::vec4 color = glm::vec4(1.0f, 0.8f, 0.2f, 1.0f);
    float trailLength = 1.0f;
    float trailWidth = 0.02f;
    bool hasTracer = true;
};

struct HitResult {
    bool hit;
    glm::vec3 point;
    glm::vec3 normal;
    float distance;
    Entity* entity;
    bool penetrated;
    
    HitResult() : hit(false), point(0.0f), normal(0.0f), distance(0.0f), entity(nullptr), penetrated(false) {}
};

class Projectile {
public:
    Projectile();
    ~Projectile();
    
    void Initialize(const ProjectileData& data, const glm::vec3& origin, const glm::vec3& direction, 
                    Entity* owner = nullptr, GameWorld* world = nullptr);
    void Update(float deltaTime);
    void Render();
    
    bool IsAlive() const { return m_alive; }
    void Destroy();
    
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetDirection() const { return m_direction; }
    float GetSpeed() const { return m_data.speed; }
    ProjectileType GetType() const { return m_data.type; }
    Entity* GetOwner() const { return m_owner; }
    
private:
    ProjectileData m_data;
    
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_velocity;
    glm::vec3 m_previousPosition;
    
    float m_lifetime;
    float m_age;
    bool m_alive;
    int m_penetrationCount;
    
    Entity* m_owner;
    GameWorld* m_world;
    
    // Trail
    std::vector<glm::vec3> m_trailPositions;
    static constexpr size_t MAX_TRAIL_LENGTH = 20;
    
    void UpdatePosition(float deltaTime);
    void CheckCollision();
    void OnHit(const HitResult& hit);
    void OnExplode(const glm::vec3& position);
    void UpdateTrail();
    void RenderTrail();
    void DealDamage(Entity* target, float damage, const glm::vec3& hitPoint);
};

class ProjectileManager {
public:
    static ProjectileManager& GetInstance();
    
    void Initialize(GameWorld* world);
    void Shutdown();
    
    void Update(float deltaTime);
    void Render();
    
    Projectile* SpawnProjectile(const ProjectileData& data, const glm::vec3& origin, 
                                const glm::vec3& direction, Entity* owner = nullptr);
    
    void DestroyProjectile(Projectile* projectile);
    void DestroyAllProjectiles();
    
    size_t GetActiveCount() const { return m_projectiles.size(); }
    
private:
    ProjectileManager() = default;
    ~ProjectileManager() = default;
    
    ProjectileManager(const ProjectileManager&) = delete;
    ProjectileManager& operator=(const ProjectileManager&) = delete;
    
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    GameWorld* m_world;
    bool m_initialized;
};

} // namespace fps::game
