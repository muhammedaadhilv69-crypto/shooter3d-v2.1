#include "renderer/Renderer.hpp"
#include "core/Logger.hpp"

namespace fps::renderer {

Renderer& Renderer::GetInstance() {
    static Renderer instance;
    return instance;
}

bool Renderer::Initialize(int width, int height) {
    LOG_INFO("Initializing renderer...");
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Set viewport
    glViewport(0, 0, width, height);
    
    // Initialize debug rendering
    InitializeDebugRendering();
    
    m_initialized = true;
    LOG_INFO("Renderer initialized successfully");
    return true;
}

void Renderer::Shutdown() {
    m_debugShader.reset();
    m_lineMesh.reset();
    m_boxMesh.reset();
    m_initialized = false;
    LOG_INFO("Renderer shutdown");
}

void Renderer::InitializeDebugRendering() {
    // Create debug shader
    m_debugShader = std::make_unique<Shader>();
    
    const std::string vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 viewProj;
        uniform mat4 model;
        void main() {
            gl_Position = viewProj * model * vec4(aPos, 1.0);
        }
    )";
    
    const std::string fragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 color;
        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";
    
    m_debugShader->LoadFromSource(vertexSource, fragmentSource);
    
    // Create line mesh
    m_lineMesh = std::make_unique<Mesh>();
    std::vector<Vertex> lineVerts = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
    };
    m_lineMesh->Create(lineVerts);
    
    // Create box mesh
    m_boxMesh = std::make_unique<Mesh>();
    m_boxMesh->CreateCube();
}

void Renderer::BeginFrame(const Camera& camera) {
    m_stats.Reset();
    
    // Clear lights
    ClearLights();
}

void Renderer::EndFrame() {
    FlushDebugDraws();
}

void Renderer::Clear(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform) {
    shader.Bind();
    shader.SetMat4("model", transform);
    mesh.Draw(shader);
    
    m_stats.drawCalls++;
    m_stats.shaderBinds++;
}

void Renderer::DrawMeshInstanced(const Mesh& mesh, const Shader& shader, const std::vector<glm::mat4>& transforms) {
    if (transforms.empty()) return;
    
    shader.Bind();
    mesh.DrawInstanced(shader, static_cast<int>(transforms.size()));
    
    m_stats.drawCalls++;
    m_stats.shaderBinds++;
    m_stats.vertices += mesh.GetVertices().size() * transforms.size();
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    m_debugLines.push_back({start, end});
}

void Renderer::DrawLines(const std::vector<glm::vec3>& points, const glm::vec3& color) {
    for (size_t i = 0; i < points.size() - 1; i += 2) {
        DrawLine(points[i], points[i + 1], color);
    }
}

void Renderer::DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) {
    glm::vec3 size = max - min;
    glm::vec3 center = (min + max) * 0.5f;
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), size);
    
    // Draw wireframe box
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_debugShader->Bind();
    m_debugShader->SetMat4("model", model);
    m_debugShader->SetVec3("color", color);
    m_boxMesh->Draw(*m_debugShader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    m_stats.drawCalls++;
}

void Renderer::DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color) {
    // Simplified: draw a wireframe sphere using lines
    const int segments = 16;
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        // XY plane circle
        glm::vec3 p1(center.x + radius * cos(angle1), center.y + radius * sin(angle1), center.z);
        glm::vec3 p2(center.x + radius * cos(angle2), center.y + radius * sin(angle2), center.z);
        DrawLine(p1, p2, color);
        
        // XZ plane circle
        p1 = glm::vec3(center.x + radius * cos(angle1), center.y, center.z + radius * sin(angle1));
        p2 = glm::vec3(center.x + radius * cos(angle2), center.y, center.z + radius * sin(angle2));
        DrawLine(p1, p2, color);
        
        // YZ plane circle
        p1 = glm::vec3(center.x, center.y + radius * cos(angle1), center.z + radius * sin(angle1));
        p2 = glm::vec3(center.x, center.y + radius * cos(angle2), center.z + radius * sin(angle2));
        DrawLine(p1, p2, color);
    }
}

void Renderer::FlushDebugDraws() {
    if (m_debugLines.empty()) return;
    
    // Create line mesh from accumulated lines
    std::vector<Vertex> lineVerts;
    for (const auto& line : m_debugLines) {
        lineVerts.push_back({line.first, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
        lineVerts.push_back({line.second, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
    }
    
    Mesh lineMesh;
    lineMesh.Create(lineVerts);
    
    // Draw all lines
    m_debugShader->Bind();
    m_debugShader->SetMat4("model", glm::mat4(1.0f));
    m_debugShader->SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVerts.size()));
    
    m_debugLines.clear();
    m_stats.drawCalls++;
}

void Renderer::SetDirectionalLight(const DirectionalLight& light) {
    m_dirLight = light;
}

void Renderer::AddPointLight(const PointLight& light) {
    if (m_pointLights.size() < MAX_POINT_LIGHTS) {
        m_pointLights.push_back(light);
    }
}

void Renderer::AddSpotLight(const SpotLight& light) {
    if (m_spotLights.size() < MAX_SPOT_LIGHTS) {
        m_spotLights.push_back(light);
    }
}

void Renderer::ClearLights() {
    m_pointLights.clear();
    m_spotLights.clear();
}

void Renderer::SetDepthTest(bool enabled) {
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::SetDepthFunc(GLenum func) {
    glDepthFunc(func);
}

void Renderer::SetBlend(bool enabled) {
    if (enabled) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::SetBlendFunc(GLenum src, GLenum dst) {
    glBlendFunc(src, dst);
}

void Renderer::SetCullFace(bool enabled) {
    if (enabled) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::SetCullFaceMode(GLenum mode) {
    glCullFace(mode);
}

void Renderer::SetWireframe(bool enabled) {
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}

void Renderer::EnableDebugOutput(bool enabled) {
    if (enabled) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, 
                                   GLsizei length, const GLchar* message, const void* userParam) {
            LOG_DEBUG("OpenGL Debug: " + std::string(message));
        }, nullptr);
    } else {
        glDisable(GL_DEBUG_OUTPUT);
    }
}

} // namespace fps::renderer
