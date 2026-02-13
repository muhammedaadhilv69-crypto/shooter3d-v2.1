#include "Renderer.h"
#include <GL/gl.h>

namespace Engine {

static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0);

    vec3 result = (ambient + diffuse + specular) * objectColor * Color;
    FragColor = vec4(result, 1.0);
}
)";

Renderer::Renderer() 
    : m_camera(nullptr)
    , m_viewportWidth(800)
    , m_viewportHeight(600)
{
}

Renderer::~Renderer() {
}

bool Renderer::init() {
    if (!m_shader.loadFromString(vertexShaderSource, fragmentShaderSource)) {
        return false;
    }
    return true;
}

void Renderer::clear(const glm::vec3& color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderMesh(Mesh& mesh, const glm::mat4& model, const glm::vec3& color) {
    if (!m_camera) return;

    m_shader.use();

    // Set matrices
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", m_camera->getViewMatrix());
    
    float aspect = (float)m_viewportWidth / (float)m_viewportHeight;
    m_shader.setMat4("projection", m_camera->getProjectionMatrix(aspect));

    // Set lighting
    m_shader.setVec3("objectColor", color);
    m_shader.setVec3("lightPos", glm::vec3(5.0f, 10.0f, 5.0f));
    m_shader.setVec3("viewPos", m_camera->getPosition());

    mesh.draw();
    m_shader.unbind();
}

void Renderer::setViewport(int width, int height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
    glViewport(0, 0, width, height);
}

} // namespace Engine
