#ifndef PARTICLE_H
#define PARTICLE_H

#include "../engine/Mesh.h"
#include <glm/glm.hpp>
#include <vector>

namespace Game {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    float life;
    float size;
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    void update(float deltaTime);
    void emit(const glm::vec3& position, const glm::vec3& direction, 
              const glm::vec3& color, int count = 10);
    
    void render(class Engine::Renderer& renderer);

private:
    std::vector<Particle> m_particles;
    Engine::Mesh m_particleMesh;
};

} // namespace Game

#endif // PARTICLE_H
