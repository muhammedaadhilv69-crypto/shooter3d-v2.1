#include "Level.h"
#include "../engine/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Game {

Level::Level()
    : m_wallMesh(Engine::Mesh::createCube(glm::vec3(0.7f, 0.7f, 0.7f)))
    , m_floorMesh(Engine::Mesh::createPlane(100.0f, glm::vec3(0.3f, 0.5f, 0.3f)))
{
}

Level::~Level() {
}

void Level::generate() {
    m_walls.clear();
    
    // Create a simple arena-style level
    float arenaSize = 20.0f;
    float wallHeight = 3.0f;
    float wallThickness = 1.0f;
    
    // Outer walls
    // North wall
    m_walls.push_back({
        glm::vec3(0.0f, wallHeight/2, -arenaSize/2),
        glm::vec3(arenaSize, wallHeight, wallThickness),
        glm::vec3(0.6f, 0.6f, 0.7f)
    });
    
    // South wall
    m_walls.push_back({
        glm::vec3(0.0f, wallHeight/2, arenaSize/2),
        glm::vec3(arenaSize, wallHeight, wallThickness),
        glm::vec3(0.6f, 0.6f, 0.7f)
    });
    
    // East wall
    m_walls.push_back({
        glm::vec3(arenaSize/2, wallHeight/2, 0.0f),
        glm::vec3(wallThickness, wallHeight, arenaSize),
        glm::vec3(0.6f, 0.6f, 0.7f)
    });
    
    // West wall
    m_walls.push_back({
        glm::vec3(-arenaSize/2, wallHeight/2, 0.0f),
        glm::vec3(wallThickness, wallHeight, arenaSize),
        glm::vec3(0.6f, 0.6f, 0.7f)
    });
    
    // Inner obstacles
    m_walls.push_back({
        glm::vec3(-5.0f, 1.0f, -5.0f),
        glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.8f, 0.4f, 0.2f)
    });
    
    m_walls.push_back({
        glm::vec3(5.0f, 1.0f, 5.0f),
        glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.8f, 0.4f, 0.2f)
    });
    
    m_walls.push_back({
        glm::vec3(-5.0f, 1.0f, 5.0f),
        glm::vec3(3.0f, 2.0f, 1.0f),
        glm::vec3(0.5f, 0.5f, 0.8f)
    });
    
    m_walls.push_back({
        glm::vec3(5.0f, 1.5f, -5.0f),
        glm::vec3(1.0f, 3.0f, 3.0f),
        glm::vec3(0.5f, 0.8f, 0.5f)
    });
    
    // Central platform
    m_walls.push_back({
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(4.0f, 1.0f, 4.0f),
        glm::vec3(0.4f, 0.4f, 0.6f)
    });
}

void Level::render(Engine::Renderer& renderer) {
    // Render floor
    glm::mat4 floorModel = glm::mat4(1.0f);
    renderer.renderMesh(m_floorMesh, floorModel, glm::vec3(0.3f, 0.5f, 0.3f));
    
    // Render walls
    for (const auto& wall : m_walls) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, wall.position);
        model = glm::scale(model, wall.scale);
        renderer.renderMesh(m_wallMesh, model, wall.color);
    }
}

} // namespace Game
