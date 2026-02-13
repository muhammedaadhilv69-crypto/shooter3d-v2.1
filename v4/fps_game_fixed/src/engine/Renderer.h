#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/glm.hpp>

namespace Engine {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void clear(const glm::vec3& color = glm::vec3(0.1f, 0.1f, 0.15f));
    
    void renderMesh(Mesh& mesh, const glm::mat4& model, const glm::vec3& color = glm::vec3(1.0f));
    void setCamera(Camera* camera) { m_camera = camera; }
    void setViewport(int width, int height);

    Shader* getShader() { return &m_shader; }

private:
    Shader m_shader;
    Camera* m_camera;
    int m_viewportWidth;
    int m_viewportHeight;
};

} // namespace Engine

#endif // RENDERER_H
