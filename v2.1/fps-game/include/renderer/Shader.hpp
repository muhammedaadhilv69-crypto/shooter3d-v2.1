#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace fps::renderer {

class Shader {
public:
    Shader();
    ~Shader();
    
    // Non-copyable
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    // Movable
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    
    bool LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    bool LoadCompute(const std::string& computePath);
    bool LoadComputeSource(const std::string& computeSource);
    
    void Bind() const;
    void Unbind() const;
    void Delete();
    
    // Uniforms
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetMat3(const std::string& name, const glm::mat3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);
    void SetBool(const std::string& name, bool value);
    
    void SetIntArray(const std::string& name, int* values, int count);
    
    GLuint GetID() const { return m_programID; }
    bool IsValid() const { return m_programID != 0; }
    
    static void UnbindAll();
    
private:
    GLuint m_programID;
    mutable std::unordered_map<std::string, GLint> m_uniformCache;
    
    bool CompileShader(GLuint shader, const std::string& source, const std::string& type);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);
    bool LinkComputeProgram(GLuint computeShader);
    std::string ReadFile(const std::string& filepath);
    GLint GetUniformLocation(const std::string& name) const;
};

} // namespace fps::renderer
