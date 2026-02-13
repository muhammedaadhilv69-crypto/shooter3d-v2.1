#pragma once

#include <string>
#include <glm/glm.hpp>
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"

namespace fps::game {

class Player;
class Weapon;

struct HUDElement {
    glm::vec2 position; // Normalized 0-1
    glm::vec2 size;
    glm::vec4 color;
    std::string textureName;
    bool visible;
    float alpha;
    
    HUDElement() : position(0.0f), size(0.1f), color(1.0f), visible(true), alpha(1.0f) {}
};

class HUD {
public:
    HUD();
    ~HUD();
    
    bool Initialize();
    void Shutdown();
    
    void Update(float deltaTime);
    void Render(int screenWidth, int screenHeight);
    
    void SetPlayer(Player* player);
    
    // Crosshair
    void SetCrosshairStyle(int style);
    void SetCrosshairColor(const glm::vec4& color);
    void SetCrosshairSize(float size);
    void ShowCrosshair(bool show);
    
    // Hit marker
    void ShowHitMarker(bool headshot = false);
    void ShowKillMarker();
    
    // Damage indicator
    void ShowDamageIndicator(const glm::vec3& damageSource);
    
    // Notifications
    void ShowNotification(const std::string& text, float duration = 3.0f);
    void ShowWeaponPickup(const std::string& weaponName);
    void ShowWaveStart(int waveNumber);
    void ShowWaveComplete(int waveNumber);
    
    // Screen effects
    void SetDamageEffect(float intensity);
    void SetLowHealthEffect(bool enabled);
    void SetADSOverlay(float opacity);
    
    // Debug
    void ShowDebugInfo(bool show);
    void SetFPS(float fps);
    void SetFrameTime(float ms);
    
private:
    Player* m_player;
    
    // Screen dimensions
    int m_screenWidth;
    int m_screenHeight;
    
    // Crosshair
    int m_crosshairStyle;
    glm::vec4 m_crosshairColor;
    float m_crosshairSize;
    float m_crosshairSpread;
    bool m_showCrosshair;
    
    // Hit marker
    float m_hitMarkerTimer;
    float m_hitMarkerDuration;
    bool m_hitMarkerHeadshot;
    
    // Kill marker
    float m_killMarkerTimer;
    float m_killMarkerDuration;
    
    // Damage indicator
    struct DamageIndicator {
        float angle;
        float timer;
        float duration;
    };
    std::vector<DamageIndicator> m_damageIndicators;
    
    // Notifications
    struct Notification {
        std::string text;
        float timer;
        float duration;
        float alpha;
    };
    std::vector<Notification> m_notifications;
    
    // Screen effects
    float m_damageEffectIntensity;
    float m_damageEffectDecay;
    bool m_lowHealthEffect;
    float m_adsOverlayOpacity;
    
    // Debug
    bool m_showDebugInfo;
    float m_fps;
    float m_frameTime;
    
    // Rendering
    std::unique_ptr<renderer::Shader> m_hudShader;
    std::unique_ptr<renderer::Shader> m_textShader;
    unsigned int m_vao;
    unsigned int m_vbo;
    
    // Textures
    std::unique_ptr<renderer::Texture2D> m_crosshairTexture;
    std::unique_ptr<renderer::Texture2D> m_hitMarkerTexture;
    std::unique_ptr<renderer::Texture2D> m_damageIndicatorTexture;
    
    void RenderCrosshair();
    void RenderHitMarker();
    void RenderDamageIndicators();
    void RenderHealthBar();
    void RenderArmorBar();
    void RenderAmmoCounter();
    void RenderWeaponInfo();
    void RenderScore();
    void RenderWaveInfo();
    void RenderNotifications();
    void RenderScreenEffects();
    void RenderDebugInfo();
    
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec4& color);
    void RenderQuad(float x, float y, float width, float height, const glm::vec4& color);
    void RenderTexturedQuad(float x, float y, float width, float height, renderer::Texture2D* texture, const glm::vec4& color = glm::vec4(1.0f));
    
    void InitializeBuffers();
    void UpdateCrosshairSpread();
};

} // namespace fps::game
