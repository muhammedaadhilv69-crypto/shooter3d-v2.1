#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Camera.hpp"

namespace fps::renderer {

struct RenderStats {
    int drawCalls = 0;
    int triangles = 0;
    int vertices = 0;
    int shaderBinds = 0;
    int textureBinds = 0;
    
    void Reset() {
        drawCalls = 0;
        triangles = 0;
        vertices = 0;
        shaderBinds = 0;
        textureBinds = 0;
    }
};

struct RenderCommand {
    enum Type {
        DrawMesh,
        DrawMeshInstanced,
        DrawLine,
        DrawPoint,
        Clear,
        SetViewport,
        SetDepthTest,
        SetBlend,
        SetCullFace
    };
    
    Type type;
    // Command data...
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    
    DirectionalLight() : direction(0.0f, -1.0f, 0.0f), color(1.0f), intensity(1.0f) {}
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float range;
    float attenuation;
    
    PointLight() : position(0.0f), color(1.0f), intensity(1.0f), range(10.0f), attenuation(1.0f) {}
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float range;
    float innerCutoff;
    float outerCutoff;
    
    SpotLight() : position(0.0f), direction(0.0f, -1.0f, 0.0f), color(1.0f), 
                  intensity(1.0f), range(10.0f), innerCutoff(0.9f), outerCutoff(0.8f) {}
};

class Renderer {
public:
    static Renderer& GetInstance();
    
    bool Initialize(int width, int height);
    void Shutdown();
    
    void BeginFrame(const Camera& camera);
    void EndFrame();
    
    void Clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void ClearDepth();
    void SetViewport(int x, int y, int width, int height);
    
    void DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& transform);
    void DrawMeshInstanced(const Mesh& mesh, const Shader& shader, const std::vector<glm::mat4>& transforms);
    
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);
    void DrawLines(const std::vector<glm::vec3>& points, const glm::vec3& color);
    void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color);
    void DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color);
    void DrawFrustum(const Camera& camera, const glm::vec3& color);
    
    // Lighting
    void SetDirectionalLight(const DirectionalLight& light);
    void AddPointLight(const PointLight& light);
    void AddSpotLight(const SpotLight& light);
    void ClearLights();
    
    // State
    void SetDepthTest(bool enabled);
    void SetDepthFunc(GLenum func);
    void SetBlend(bool enabled);
    void SetBlendFunc(GLenum src, GLenum dst);
    void SetCullFace(bool enabled);
    void SetCullFaceMode(GLenum mode);
    void SetWireframe(bool enabled);
    
    const RenderStats& GetStats() const { return m_stats; }
    void ResetStats() { m_stats.Reset(); }
    
    // Debug
    void EnableDebugOutput(bool enabled);
    
private:
    Renderer() = default;
    ~Renderer() = default;
    
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    RenderStats m_stats;
    
    std::unique_ptr<Mesh> m_lineMesh;
    std::unique_ptr<Mesh> m_boxMesh;
    std::unique_ptr<Shader> m_debugShader;
    
    DirectionalLight m_dirLight;
    std::vector<PointLight> m_pointLights;
    std::vector<SpotLight> m_spotLights;
    static constexpr size_t MAX_POINT_LIGHTS = 8;
    static constexpr size_t MAX_SPOT_LIGHTS = 4;
    
    bool m_initialized;
    
    void InitializeDebugRendering();
    void FlushDebugDraws();
    
    std::vector<std::pair<glm::vec3, glm::vec3>> m_debugLines;
};

} // namespace fps::renderer
