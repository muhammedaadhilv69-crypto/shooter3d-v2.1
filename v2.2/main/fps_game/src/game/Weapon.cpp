#include "Weapon.h"
#include <iostream>
#include <algorithm>

namespace Game {

Weapon::Weapon(WeaponType type)
    : m_type(type)
    , m_currentAmmo(30)
    , m_maxAmmo(30)
    , m_reserveAmmo(90)
    , m_fireRate(0.1f)
    , m_damage(25.0f)
    , m_reloadTime(2.0f)
    , m_timeSinceLastShot(0.0f)
    , m_reloadTimer(0.0f)
    , m_reloading(false)
    , m_position(0.0f)
    , m_barrelPos(0.0f)
    , m_direction(0.0f, 0.0f, -1.0f)
{
    // Create weapon mesh (simple box for now)
    m_mesh = Engine::Mesh::createCube(glm::vec3(0.3f, 0.3f, 0.3f));
    
    // Customize based on type
    switch (m_type) {
        case WeaponType::PISTOL:
            m_maxAmmo = 12;
            m_currentAmmo = 12;
            m_reserveAmmo = 48;
            m_fireRate = 0.2f;
            m_damage = 35.0f;
            m_reloadTime = 1.5f;
            break;
            
        case WeaponType::RIFLE:
            m_maxAmmo = 30;
            m_currentAmmo = 30;
            m_reserveAmmo = 120;
            m_fireRate = 0.1f;
            m_damage = 25.0f;
            m_reloadTime = 2.0f;
            break;
            
        case WeaponType::SHOTGUN:
            m_maxAmmo = 8;
            m_currentAmmo = 8;
            m_reserveAmmo = 32;
            m_fireRate = 0.8f;
            m_damage = 80.0f;
            m_reloadTime = 2.5f;
            break;
    }
}

Weapon::~Weapon() {
}

void Weapon::update(float deltaTime) {
    m_timeSinceLastShot += deltaTime;
    
    if (m_reloading) {
        m_reloadTimer += deltaTime;
        if (m_reloadTimer >= m_reloadTime) {
            int ammoNeeded = m_maxAmmo - m_currentAmmo;
            int ammoToReload = std::min(ammoNeeded, m_reserveAmmo);
            m_currentAmmo += ammoToReload;
            m_reserveAmmo -= ammoToReload;
            m_reloading = false;
            m_reloadTimer = 0.0f;
            std::cout << "Reload complete! Ammo: " << m_currentAmmo << "/" << m_reserveAmmo << std::endl;
        }
    }
}

void Weapon::fire() {
    if (!canFire()) return;
    
    m_currentAmmo--;
    m_timeSinceLastShot = 0.0f;
    
    std::cout << "BANG! Ammo: " << m_currentAmmo << "/" << m_reserveAmmo << std::endl;
    
    if (m_currentAmmo == 0) {
        reload();
    }
}

void Weapon::reload() {
    if (m_reloading || m_currentAmmo == m_maxAmmo || m_reserveAmmo == 0) return;
    
    m_reloading = true;
    m_reloadTimer = 0.0f;
    std::cout << "Reloading..." << std::endl;
}

bool Weapon::canFire() const {
    return !m_reloading && m_currentAmmo > 0 && m_timeSinceLastShot >= m_fireRate;
}

glm::mat4 Weapon::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.8f));
    return model;
}

} // namespace Game
