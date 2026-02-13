#include "renderer/Shader.hpp"
#include "core/Logger.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace fps::renderer {

Shader::Shader() : m_programID(0) {
}

Shader::~Shader() {
    Delete();
}

Shader::Shader(Shader&& other) noexcept : m_programID(other.m_programID), m_uniformCache(std::move(other.m_uniformCache)) {
    other.m_programID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        Delete();
        m_programID = other.m_programID;
        m_uniformCache = std::move(other.m_uniformCache);
        other.m_programID = 0;
    }
    return *this;
}

void Shader::Delete() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
    m_uniformCache.clear();
}

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open shader file: " + filepath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Shader::CompileShader(GLuint shader, const std::string& source, const std::string& type) {
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LOG_ERROR("Shader compilation failed (" + type + "): " + std::string(infoLog));
        return false;
    }
    
    return true;
}

bool Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);
    
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        LOG_ERROR("Shader program linking failed: " + std::string(infoLog));
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }
    
    return true;
}

bool Shader::LinkComputeProgram(GLuint computeShader) {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, computeShader);
    glLinkProgram(m_programID);
    
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        LOG_ERROR("Compute shader program linking failed: " + std::string(infoLog));
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }
    
    return true;
}

bool Shader::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        return false;
    }
    
    return LoadFromSource(vertexSource, fragmentSource);
}

bool Shader::LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    if (!CompileShader(vertexShader, vertexSource, "vertex") ||
        !CompileShader(fragmentShader, fragmentSource, "fragment")) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    if (!LinkProgram(vertexShader, fragmentShader)) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    LOG_DEBUG("Shader program created successfully (ID: " + std::to_string(m_programID) + ")");
    return true;
}

bool Shader::LoadCompute(const std::string& computePath) {
    std::string computeSource = ReadFile(computePath);
    if (computeSource.empty()) {
        return false;
    }
    return LoadComputeSource(computeSource);
}

bool Shader::LoadComputeSource(const std::string& computeSource) {
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    
    if (!CompileShader(computeShader, computeSource, "compute")) {
        glDeleteShader(computeShader);
        return false;
    }
    
    if (!LinkComputeProgram(computeShader)) {
        glDeleteShader(computeShader);
        return false;
    }
    
    glDeleteShader(computeShader);
    
    LOG_DEBUG("Compute shader program created successfully (ID: " + std::to_string(m_programID) + ")");
    return true;
}

void Shader::Bind() const {
    glUseProgram(m_programID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::UnbindAll() {
    glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string& name) const {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }
    
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1) {
        LOG_WARNING("Uniform '" + name + "' not found in shader");
    }
    
    m_uniformCache[name] = location;
    return location;
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetBool(const std::string& name, bool value) {
    glUniform1i(GetUniformLocation(name), value ? 1 : 0);
}

void Shader::SetIntArray(const std::string& name, int* values, int count) {
    glUniform1iv(GetUniformLocation(name), count, values);
}

} // namespace fps::renderer
