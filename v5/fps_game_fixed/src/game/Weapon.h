#ifndef WEAPON_H
#define WEAPON_H

#include "../engine/Mesh.h"
#include <glm/glm.hpp>
#include <string>

namespace Game {

enum class WeaponType {
    PISTOL,
    RIFLE,
    SHOTGUN
};

class Weapon {
public:
    Weapon(WeaponType type);
    ~Weapon();

    void update(float deltaTime);
    void fire();
    void reload();

    bool canFire() const;
    int getAmmo() const { return m_currentAmmo; }
    int getReserveAmmo() const { return m_reserveAmmo; }
    bool isReloading() const { return m_reloading; }
    
    glm::vec3 getBarrelPosition() const { return m_barrelPos; }
    glm::vec3 getDirection() const { return m_direction; }
    
    Engine::Mesh& getMesh() { return m_mesh; }
    glm::mat4 getModelMatrix() const;

private:
    WeaponType m_type;
    Engine::Mesh m_mesh;
    
    int m_currentAmmo;
    int m_maxAmmo;
    int m_reserveAmmo;
    
    float m_fireRate;
    float m_damage;
    float m_reloadTime;
    
    float m_timeSinceLastShot;
    float m_reloadTimer;
    bool m_reloading;
    
    glm::vec3 m_position;
    glm::vec3 m_barrelPos;
    glm::vec3 m_direction;
};

} // namespace Game

#endif // WEAPON_H
