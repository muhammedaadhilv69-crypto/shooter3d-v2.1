#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace Engine {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 color;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void draw();
    void cleanup();

    static Mesh createCube(const glm::vec3& color = glm::vec3(1.0f));
    static Mesh createPlane(float size, const glm::vec3& color = glm::vec3(1.0f));
    static Mesh createSphere(float radius, int segments, const glm::vec3& color = glm::vec3(1.0f));

private:
    GLuint m_VAO, m_VBO, m_EBO;
    unsigned int m_indexCount;
    bool m_initialized;
};

} // namespace Engine

#endif // MESH_H
