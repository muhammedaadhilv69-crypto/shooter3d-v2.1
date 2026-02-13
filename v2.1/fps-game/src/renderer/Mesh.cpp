#include "renderer/Mesh.hpp"
#include "core/Logger.hpp"
#include <cmath>

namespace fps::renderer {

Mesh::Mesh() : m_vao(0), m_vbo(0), m_ebo(0), m_vertexCount(0), m_indexCount(0) {
}

Mesh::~Mesh() {
    Delete();
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao)
    , m_vbo(other.m_vbo)
    , m_ebo(other.m_ebo)
    , m_vertices(std::move(other.m_vertices))
    , m_indices(std::move(other.m_indices))
    , m_vertexCount(other.m_vertexCount)
    , m_indexCount(other.m_indexCount) {
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        Delete();
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_vertices = std::move(other.m_vertices);
        m_indices = std::move(other.m_indices);
        m_vertexCount = other.m_vertexCount;
        m_indexCount = other.m_indexCount;
        other.m_vao = 0;
        other.m_vbo = 0;
        other.m_ebo = 0;
    }
    return *this;
}

void Mesh::Delete() {
    if (m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

void Mesh::SetupMesh() {
    if (m_vao != 0) {
        Delete();
    }
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    
    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // TexCoord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    
    // Tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    
    // Bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    
    // Index buffer
    if (!m_indices.empty()) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
        m_indexCount = m_indices.size();
    }
    
    m_vertexCount = m_vertices.size();
    
    glBindVertexArray(0);
}

void Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    m_vertices = vertices;
    m_indices = indices;
    SetupMesh();
}

void Mesh::Create(const std::vector<Vertex>& vertices) {
    m_vertices = vertices;
    m_indices.clear();
    SetupMesh();
}

void Mesh::CreateQuad() {
    m_vertices = {
        // Position              Normal           TexCoord    Tangent    Bitangent
        { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }
    };
    
    m_indices = { 0, 1, 2, 0, 2, 3 };
    SetupMesh();
}

void Mesh::CreateCube() {
    m_vertices = {
        // Front face
        { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        // Back face
        { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        // Top face
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        // Bottom face
        { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        // Right face
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        // Left face
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f} }
    };
    
    m_indices = {
        0, 1, 2, 0, 2, 3,       // Front
        4, 5, 6, 4, 6, 7,       // Back
        8, 9, 10, 8, 10, 11,    // Top
        12, 13, 14, 12, 14, 15, // Bottom
        16, 17, 18, 16, 18, 19, // Right
        20, 21, 22, 20, 22, 23  // Left
    };
    
    SetupMesh();
}

void Mesh::CreateSphere(float radius, int segments, int rings) {
    GenerateSphereVertices(radius, segments, rings);
    SetupMesh();
}

void Mesh::GenerateSphereVertices(float radius, int segments, int rings) {
    m_vertices.clear();
    m_indices.clear();
    
    for (int ring = 0; ring <= rings; ++ring) {
        float phi = M_PI * float(ring) / float(rings);
        
        for (int seg = 0; seg <= segments; ++seg) {
            float theta = 2.0f * M_PI * float(seg) / float(segments);
            
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);
            
            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position);
            
            float u = float(seg) / float(segments);
            float v = float(ring) / float(rings);
            
            // Calculate tangent
            float tx = -std::sin(theta);
            float ty = 0.0f;
            float tz = std::cos(theta);
            glm::vec3 tangent(tx, ty, tz);
            
            glm::vec3 bitangent = glm::cross(normal, tangent);
            
            m_vertices.push_back({position, normal, {u, v}, tangent, bitangent});
        }
    }
    
    for (int ring = 0; ring < rings; ++ring) {
        for (int seg = 0; seg < segments; ++seg) {
            int current = ring * (segments + 1) + seg;
            int next = current + segments + 1;
            
            m_indices.push_back(current);
            m_indices.push_back(next);
            m_indices.push_back(current + 1);
            
            m_indices.push_back(current + 1);
            m_indices.push_back(next);
            m_indices.push_back(next + 1);
        }
    }
}

void Mesh::CreateCylinder(float radius, float height, int segments) {
    GenerateCylinderVertices(radius, height, segments);
    SetupMesh();
}

void Mesh::GenerateCylinderVertices(float radius, float height, int segments) {
    m_vertices.clear();
    m_indices.clear();
    
    float halfHeight = height * 0.5f;
    
    // Side vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        
        glm::vec3 normal(x / radius, 0.0f, z / radius);
        glm::vec3 tangent(-std::sin(angle), 0.0f, std::cos(angle));
        glm::vec3 bitangent = glm::cross(normal, tangent);
        
        // Bottom ring
        m_vertices.push_back({{x, -halfHeight, z}, normal, {float(i) / segments, 0.0f}, tangent, bitangent});
        // Top ring
        m_vertices.push_back({{x, halfHeight, z}, normal, {float(i) / segments, 1.0f}, tangent, bitangent});
    }
    
    // Side indices
    for (int i = 0; i < segments; ++i) {
        int bottomCurrent = i * 2;
        int topCurrent = bottomCurrent + 1;
        int bottomNext = bottomCurrent + 2;
        int topNext = topCurrent + 2;
        
        m_indices.push_back(bottomCurrent);
        m_indices.push_back(bottomNext);
        m_indices.push_back(topCurrent);
        
        m_indices.push_back(topCurrent);
        m_indices.push_back(bottomNext);
        m_indices.push_back(topNext);
    }
    
    // TODO: Add caps
}

void Mesh::CalculateTangents() {
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        Vertex& v0 = m_vertices[m_indices[i]];
        Vertex& v1 = m_vertices[m_indices[i + 1]];
        Vertex& v2 = m_vertices[m_indices[i + 2]];
        
        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        
        glm::vec2 deltaUV1 = v1.texCoords - v0.texCoords;
        glm::vec2 deltaUV2 = v2.texCoords - v0.texCoords;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        
        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;
        
        glm::vec3 bitangent = glm::cross(v0.normal, tangent);
        v0.bitangent = bitangent;
        v1.bitangent = bitangent;
        v2.bitangent = bitangent;
    }
}

void Mesh::Draw(const Shader& shader) const {
    glBindVertexArray(m_vao);
    
    if (m_indexCount > 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount));
    }
    
    glBindVertexArray(0);
}

void Mesh::DrawInstanced(const Shader& shader, int instanceCount) const {
    glBindVertexArray(m_vao);
    
    if (m_indexCount > 0) {
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr, instanceCount);
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount), instanceCount);
    }
    
    glBindVertexArray(0);
}

void Mesh::CalculateAABB(const std::vector<Vertex>& vertices, glm::vec3& min, glm::vec3& max) {
    if (vertices.empty()) {
        min = max = glm::vec3(0.0f);
        return;
    }
    
    min = max = vertices[0].position;
    
    for (const auto& vertex : vertices) {
        min = glm::min(min, vertex.position);
        max = glm::max(max, vertex.position);
    }
}

} // namespace fps::renderer
