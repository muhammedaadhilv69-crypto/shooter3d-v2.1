#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "renderer/Shader.hpp"

namespace fps::renderer {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    
    Vertex() = default;
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv)
        : position(pos), normal(norm), texCoords(uv), tangent(0.0f), bitangent(0.0f) {}
};

struct SubMesh {
    unsigned int baseVertex;
    unsigned int baseIndex;
    unsigned int indexCount;
    unsigned int materialIndex;
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    
    void Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void Create(const std::vector<Vertex>& vertices);
    void CreateQuad();
    void CreateCube();
    void CreateSphere(float radius = 1.0f, int segments = 32, int rings = 16);
    void CreateCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    void CreateCapsule(float radius = 1.0f, float height = 2.0f, int segments = 32, int rings = 16);
    
    void Draw(const Shader& shader) const;
    void DrawInstanced(const Shader& shader, int instanceCount) const;
    
    void Delete();
    
    const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    const std::vector<unsigned int>& GetIndices() const { return m_indices; }
    
    void CalculateTangents();
    
    bool IsValid() const { return m_vao != 0; }
    
    static void CalculateAABB(const std::vector<Vertex>& vertices, glm::vec3& min, glm::vec3& max);
    
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    size_t m_vertexCount;
    size_t m_indexCount;
    
    void SetupMesh();
    void GenerateSphereVertices(float radius, int segments, int rings);
    void GenerateCylinderVertices(float radius, float height, int segments);
};

} // namespace fps::renderer
