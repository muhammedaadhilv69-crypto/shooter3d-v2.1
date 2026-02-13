#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromString(const std::string& vertexSrc, const std::string& fragmentSrc);
    void use();
    void unbind();

    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat4(const std::string& name, const glm::mat4& value);

    GLuint getID() const { return m_program; }

private:
    GLuint m_program;
    GLuint compileShader(GLenum type, const std::string& source);
    bool linkProgram(GLuint vertex, GLuint fragment);
    GLint getUniformLocation(const std::string& name);
};

} // namespace Engine

#endif // SHADER_H
