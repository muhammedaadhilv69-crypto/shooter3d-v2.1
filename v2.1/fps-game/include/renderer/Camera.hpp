#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace fps::renderer {

enum class CameraType {
    Perspective,
    Orthographic
};

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera {
public:
    Camera();
    Camera(CameraType type, float fov, float aspectRatio, float nearPlane, float farPlane);
    
    void SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void SetOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetEulerAngles(float pitch, float yaw, float roll = 0.0f);
    
    void LookAt(const glm::vec3& target, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
    
    void Move(const glm::vec3& delta);
    void Move(CameraMovement direction, float deltaTime, float speed);
    
    void Rotate(float pitchDelta, float yawDelta);
    void RotateAround(const glm::vec3& target, float pitchDelta, float yawDelta);
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;
    
    glm::vec3 GetPosition() const { return m_position; }
    glm::quat GetRotation() const { return m_rotation; }
    glm::vec3 GetEulerAngles() const;
    
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    
    float GetFOV() const { return m_fov; }
    float GetAspectRatio() const { return m_aspectRatio; }
    float GetNearPlane() const { return m_nearPlane; }
    float GetFarPlane() const { return m_farPlane; }
    
    void SetFOV(float fov);
    void SetAspectRatio(float aspectRatio);
    void SetClipPlanes(float nearPlane, float farPlane);
    
    // Frustum
    bool IsPointInFrustum(const glm::vec3& point) const;
    bool IsSphereInFrustum(const glm::vec3& center, float radius) const;
    bool IsAABBInFrustum(const glm::vec3& min, const glm::vec3& max) const;
    
    // FPS specific
    void SetFPSMode(bool enabled) { m_fpsMode = enabled; }
    void SetPitchLimits(float minPitch, float maxPitch);
    
    // Ray casting
    glm::vec3 ScreenToWorldRay(float screenX, float screenY, float screenWidth, float screenHeight) const;
    
private:
    CameraType m_type;
    
    glm::vec3 m_position;
    glm::quat m_rotation;
    
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    
    // Orthographic params
    float m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop;
    
    // FPS mode
    bool m_fpsMode;
    float m_pitch;
    float m_yaw;
    float m_minPitch;
    float m_maxPitch;
    
    void UpdateVectors();
    void UpdateFrustumPlanes();
    
    // Frustum planes
    glm::vec4 m_frustumPlanes[6];
};

// FPS Camera controller
class FPSCameraController {
public:
    FPSCameraController(Camera* camera);
    
    void Update(float deltaTime);
    
    void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    void SetSmoothing(float smoothing) { m_smoothing = smoothing; }
    
    void OnMouseMove(float deltaX, float deltaY);
    void OnMouseScroll(float delta);
    
    void SetCanMove(bool canMove) { m_canMove = canMove; }
    void SetCanLook(bool canLook) { m_canLook = canLook; }
    
    Camera* GetCamera() const { return m_camera; }
    
private:
    Camera* m_camera;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_smoothing;
    
    float m_targetPitch;
    float m_targetYaw;
    float m_currentPitch;
    float m_currentYaw;
    
    bool m_canMove;
    bool m_canLook;
};

} // namespace fps::renderer
