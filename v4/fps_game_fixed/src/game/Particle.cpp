#include "Particle.h"
#include "../engine/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <ctime>

namespace Game {

ParticleSystem::ParticleSystem()
    : m_particleMesh(Engine::Mesh::createCube(glm::vec3(1.0f)))
{
    srand(time(nullptr));
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::update(float deltaTime) {
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        it->life -= deltaTime;
        
        if (it->life <= 0.0f) {
            it = m_particles.erase(it);
        } else {
            // Update physics
            it->velocity.y -= 9.8f * deltaTime; // Gravity
            it->position += it->velocity * deltaTime;
            
            // Fade out
            it->color *= 0.98f;
            it->size *= 0.98f;
            
            ++it;
        }
    }
}

void ParticleSystem::emit(const glm::vec3& position, const glm::vec3& direction, 
                          const glm::vec3& color, int count) {
    for (int i = 0; i < count; ++i) {
        Particle particle;
        particle.position = position;
        
        // Random spread
        float spread = 0.5f;
        particle.velocity = direction + glm::vec3(
            (rand() % 100 - 50) / 50.0f * spread,
            (rand() % 100 - 50) / 50.0f * spread,
            (rand() % 100 - 50) / 50.0f * spread
        );
        particle.velocity *= 2.0f + (rand() % 100) / 100.0f;
        
        particle.color = color;
        particle.life = 0.5f + (rand() % 100) / 200.0f;
        particle.size = 0.05f + (rand() % 50) / 500.0f;
        
        m_particles.push_back(particle);
    }
}

void ParticleSystem::render(Engine::Renderer& renderer) {
    for (const auto& particle : m_particles) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, particle.position);
        model = glm::scale(model, glm::vec3(particle.size));
        
        renderer.renderMesh(m_particleMesh, model, particle.color);
    }
}

} // namespace Game
