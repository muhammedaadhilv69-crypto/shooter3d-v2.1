#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"

namespace fps::renderer {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float size;
    float life;
    float maxLife;
    float rotation;
    float rotationSpeed;
    bool active;
    
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), size(1.0f), 
                 life(0.0f), maxLife(1.0f), rotation(0.0f), rotationSpeed(0.0f), active(false) {}
};

enum class EmitterShape {
    Point,
    Sphere,
    Box,
    Cone,
    Circle
};

struct EmitterConfig {
    EmitterShape shape = EmitterShape::Point;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);
    float spread = 0.0f; // Angle in radians for cone
    float radius = 1.0f; // For sphere/circle
    glm::vec3 boxSize = glm::vec3(1.0f); // For box
    
    float emissionRate = 100.0f; // Particles per second
    float burstCount = 0.0f; // Instant burst
    
    float minLife = 1.0f;
    float maxLife = 2.0f;
    
    float minSize = 0.1f;
    float maxSize = 0.5f;
    
    float minSpeed = 1.0f;
    float maxSpeed = 5.0f;
    
    glm::vec4 startColor = glm::vec4(1.0f);
    glm::vec4 endColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    
    float gravity = -9.81f;
    float drag = 0.0f;
    
    bool loop = true;
    float duration = 5.0f;
};

class ParticleEmitter {
public:
    ParticleEmitter();
    ~ParticleEmitter();
    
    void Initialize(const EmitterConfig& config, size_t maxParticles = 1000);
    void Update(float deltaTime);
    void Render(const Shader& shader, const glm::mat4& viewProj);
    
    void Play();
    void Stop();
    void Reset();
    void EmitBurst(int count);
    
    void SetPosition(const glm::vec3& position);
    void SetDirection(const glm::vec3& direction);
    
    bool IsPlaying() const { return m_playing; }
    bool IsAlive() const;
    
    size_t GetActiveParticleCount() const;
    
private:
    EmitterConfig m_config;
    std::vector<Particle> m_particles;
    std::vector<float> m_instanceData; // Position, size, color, rotation
    
    size_t m_maxParticles;
    size_t m_activeCount;
    
    float m_emissionAccumulator;
    float m_age;
    bool m_playing;
    
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_instanceVbo;
    
    std::unique_ptr<Mesh> m_quadMesh;
    
    void SpawnParticle();
    void InitializeBuffers();
    void UpdateInstanceData();
    glm::vec3 GetRandomPositionInShape();
    glm::vec3 GetRandomDirection();
};

class ParticleSystem {
public:
    static ParticleSystem& GetInstance();
    
    void Initialize();
    void Shutdown();
    
    void Update(float deltaTime);
    void Render(const glm::mat4& viewProj);
    
    ParticleEmitter* CreateEmitter(const EmitterConfig& config, size_t maxParticles = 1000);
    void DestroyEmitter(ParticleEmitter* emitter);
    void DestroyAllEmitters();
    
    void PlayAll();
    void StopAll();
    
private:
    ParticleSystem() = default;
    ~ParticleSystem() = default;
    
    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;
    
    std::vector<std::unique_ptr<ParticleEmitter>> m_emitters;
    std::unique_ptr<Shader> m_particleShader;
    
    bool m_initialized;
};

} // namespace fps::renderer
