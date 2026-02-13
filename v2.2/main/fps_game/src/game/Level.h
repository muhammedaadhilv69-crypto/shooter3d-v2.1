#ifndef LEVEL_H
#define LEVEL_H

#include "../engine/Mesh.h"
#include <glm/glm.hpp>
#include <vector>

namespace Game {

struct Wall {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
};

class Level {
public:
    Level();
    ~Level();

    void generate();
    void render(class Engine::Renderer& renderer);
    
    const std::vector<Wall>& getWalls() const { return m_walls; }

private:
    std::vector<Wall> m_walls;
    Engine::Mesh m_wallMesh;
    Engine::Mesh m_floorMesh;
};

} // namespace Game

#endif // LEVEL_H
