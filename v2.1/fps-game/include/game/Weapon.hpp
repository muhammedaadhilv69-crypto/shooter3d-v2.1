#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "renderer/Texture.hpp"
#include "renderer/ParticleSystem.hpp"

namespace fps::game {

class Player;
class Projectile;

enum class WeaponType {
    Pistol,
    Rifle,
    Shotgun,
    Sniper,
    SMG,
    RocketLauncher,
    Melee
};

enum class FireMode {
    Single,
    Burst,
    Automatic
};

struct WeaponStats {
    // Damage
    float damage = 25.0f;
    float headshotMultiplier = 2.0f;
    float range = 100.0f;
    float falloffStart = 50.0f;
    float falloffEnd = 100.0f;
    
    // Fire rate
    float fireRate = 10.0f; // Shots per second
    FireMode fireMode = FireMode::Single;
    int burstCount = 3;
    
    // Ammo
    int magazineSize = 30;
    int reserveAmmo = 90;
    int currentAmmo = 30;
    
    // Accuracy
    float baseSpread = 0.01f;
    float movingSpread = 0.05f;
    float jumpingSpread = 0.1f;
    float recoilRecovery = 5.0f;
    
    // Recoil
    float recoilVertical = 0.5f;
    float recoilHorizontal = 0.2f;
    float recoilPatternX[10];
    float recoilPatternY[10];
    
    // Handling
    float reloadTime = 2.0f;
    float equipTime = 0.5f;
    float aimDownSightsTime = 0.2f;
    float movementSpeedMultiplier = 1.0f;
    
    // Visual
    float muzzleFlashDuration = 0.05f;
    int tracerRounds = 1; // Every Nth round
    
    WeaponStats() {
        for (int i = 0; i < 10; i++) {
            recoilPatternX[i] = (i % 2 == 0 ? 1.0f : -1.0f) * 0.1f * i;
            recoilPatternY[i] = 0.5f + 0.1f * i;
        }
    }
};

struct WeaponVisuals {
    std::string name = "Weapon";
    std::string modelPath;
    std::string texturePath;
    std::string normalMapPath;
    std::string muzzleFlashTexture;
    std::string tracerTexture;
    std::string shellCasingModel;
    
    glm::vec3 positionOffset = glm::vec3(0.3f, -0.3f, -0.5f);
    glm::vec3 adsOffset = glm::vec3(0.0f, -0.15f, -0.3f);
    glm::vec3 rotationOffset = glm::vec3(0.0f);
    
    float fovMultiplier = 0.8f;
    float adsFovMultiplier = 0.6f;
};

class Weapon {
public:
    Weapon(WeaponType type);
    virtual ~Weapon();
    
    virtual bool Initialize();
    virtual void Shutdown();
    
    virtual void Update(float deltaTime);
    virtual void Render(const glm::mat4& viewProj);
    virtual void RenderViewModel(const glm::mat4& viewProj);
    
    // Actions
    virtual void Fire(const glm::vec3& origin, const glm::vec3& direction);
    virtual void StartFiring();
    virtual void StopFiring();
    virtual void Reload();
    virtual void Equip();
    virtual void Unequip();
    virtual void AimDownSights(bool aiming);
    
    // State checks
    bool CanFire() const;
    bool CanReload() const;
    bool IsFiring() const { return m_firing; }
    bool IsReloading() const { return m_reloading; }
    bool IsEquipping() const { return m_equipping; }
    bool IsAimingDownSights() const { return m_aimingDownSights; }
    
    // Stats
    WeaponStats& GetStats() { return m_stats; }
    const WeaponStats& GetStats() const { return m_stats; }
    WeaponType GetType() const { return m_type; }
    const std::string& GetName() const { return m_visuals.name; }
    
    // Ammo
    int GetCurrentAmmo() const { return m_stats.currentAmmo; }
    int GetReserveAmmo() const { return m_stats.reserveAmmo; }
    int GetMagazineSize() const { return m_stats.magazineSize; }
    void AddAmmo(int amount);
    
    // Visuals
    void SetVisuals(const WeaponVisuals& visuals) { m_visuals = visuals; }
    
    // Recoil
    glm::vec2 GetRecoilOffset() const;
    void ApplyRecoil();
    
    // Spread
    float GetCurrentSpread(bool moving, bool jumping) const;
    glm::vec3 ApplySpread(const glm::vec3& direction, float spread);
    
protected:
    WeaponType m_type;
    WeaponStats m_stats;
    WeaponVisuals m_visuals;
    
    // State
    bool m_firing;
    bool m_reloading;
    bool m_equipping;
    bool m_aimingDownSights;
    
    // Timers
    float m_fireTimer;
    float m_reloadTimer;
    float m_equipTimer;
    float m_adsTimer;
    float m_muzzleFlashTimer;
    
    // Recoil
    int m_shotCount;
    float m_recoilRecoveryTimer;
    glm::vec2 m_currentRecoil;
    glm::vec2 m_targetRecoil;
    
    // Visual effects
    std::unique_ptr<renderer::ParticleEmitter> m_muzzleFlash;
    std::unique_ptr<renderer::ParticleEmitter> m_shellEjection;
    
    // Audio (placeholder for now)
    // std::shared_ptr<AudioClip> m_fireSound;
    // std::shared_ptr<AudioClip> m_reloadSound;
    
    void UpdateTimers(float deltaTime);
    void UpdateRecoil(float deltaTime);
    void SpawnProjectile(const glm::vec3& origin, const glm::vec3& direction);
    void SpawnEffects(const glm::vec3& origin, const glm::vec3& direction);
    void EjectShell();
    
    virtual void OnFire();
    virtual void OnReload();
    virtual void OnEquip();
};

// Factory function
std::shared_ptr<Weapon> CreateWeapon(WeaponType type);

} // namespace fps::game
