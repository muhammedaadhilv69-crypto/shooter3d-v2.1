#include "Shader.h"
#include <iostream>
#include <vector>

namespace Engine {

Shader::Shader() : m_program(0) {
}

Shader::~Shader() {
    if (m_program) {
        glDeleteProgram(m_program);
    }
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(shader, length, &length, log.data());
        std::cerr << "Shader compilation failed:\n" << log.data() << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::linkProgram(GLuint vertex, GLuint fragment) {
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);

    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint length;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(m_program, length, &length, log.data());
        std::cerr << "Shader linking failed:\n" << log.data() << std::endl;
        return false;
    }

    return true;
}

bool Shader::loadFromString(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (!vertexShader) return false;

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return false;
    }

    if (!linkProgram(vertexShader, fragmentShader)) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::use() {
    glUseProgram(m_program);
}

void Shader::unbind() {
    glUseProgram(0);
}

GLint Shader::getUniformLocation(const std::string& name) {
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace Engine
