#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getFront() const { return m_front; }
    glm::vec3 getUp() const { return m_up; }
    glm::vec3 getRight() const { return m_right; }
    
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }
    float getFOV() const { return m_fov; }

    void setPosition(const glm::vec3& pos) { m_position = pos; }
    void setYaw(float yaw) { m_yaw = yaw; updateCameraVectors(); }
    void setPitch(float pitch) { m_pitch = pitch; updateCameraVectors(); }

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_fov;

    void updateCameraVectors();
};

} // namespace Engine

#endif // CAMERA_H
