#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "math/Math.hpp"

namespace fps::game {

using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY_ID = 0;

enum class EntityType {
    None,
    Player,
    Enemy,
    Weapon,
    Projectile,
    Pickup,
    Prop,
    Trigger
};

class Entity {
public:
    Entity(EntityType type = EntityType::None);
    virtual ~Entity();
    
    virtual void Update(float deltaTime) = 0;
    virtual void Render(const glm::mat4& viewProj) = 0;
    
    EntityID GetID() const { return m_id; }
    EntityType GetType() const { return m_type; }
    
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);
    
    glm::vec3 GetPosition() const { return m_transform.position; }
    glm::quat GetRotation() const { return m_transform.rotation; }
    glm::vec3 GetScale() const { return m_transform.scale; }
    const math::Transform& GetTransform() const { return m_transform; }
    
    glm::vec3 GetForward() const { return m_transform.GetForward(); }
    glm::vec3 GetRight() const { return m_transform.GetRight(); }
    glm::vec3 GetUp() const { return m_transform.GetUp(); }
    
    void SetActive(bool active) { m_active = active; }
    bool IsActive() const { return m_active; }
    
    void MarkForDestruction() { m_markedForDestruction = true; }
    bool IsMarkedForDestruction() const { return m_markedForDestruction; }
    
    // Collision
    virtual math::AABB GetBoundingBox() const;
    virtual math::Sphere GetBoundingSphere() const;
    virtual bool CheckCollision(const Entity& other) const;
    
protected:
    EntityID m_id;
    EntityType m_type;
    math::Transform m_transform;
    bool m_active;
    bool m_markedForDestruction;
    
    static EntityID s_nextID;
};

} // namespace fps::game
