#include "renderer/Camera.hpp"
#include "core/Logger.hpp"
#include <cmath>

namespace fps::renderer {

Camera::Camera()
    : m_type(CameraType::Perspective)
    , m_position(0.0f, 0.0f, 3.0f)
    , m_rotation(1.0f, 0.0f, 0.0f, 0.0f)
    , m_fov(60.0f)
    , m_aspectRatio(16.0f / 9.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_orthoLeft(-10.0f)
    , m_orthoRight(10.0f)
    , m_orthoBottom(-10.0f)
    , m_orthoTop(10.0f)
    , m_fpsMode(false)
    , m_pitch(0.0f)
    , m_yaw(-90.0f)
    , m_minPitch(-89.0f)
    , m_maxPitch(89.0f) {
    UpdateVectors();
}

Camera::Camera(CameraType type, float fov, float aspectRatio, float nearPlane, float farPlane)
    : Camera() {
    m_type = type;
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    m_type = CameraType::Perspective;
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    m_type = CameraType::Orthographic;
    m_orthoLeft = left;
    m_orthoRight = right;
    m_orthoBottom = bottom;
    m_orthoTop = top;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetPosition(const glm::vec3& position) {
    m_position = position;
}

void Camera::SetRotation(const glm::quat& rotation) {
    m_rotation = rotation;
    glm::vec3 euler = glm::eulerAngles(rotation);
    m_pitch = glm::degrees(euler.x);
    m_yaw = glm::degrees(euler.y);
}

void Camera::SetEulerAngles(float pitch, float yaw, float roll) {
    m_pitch = glm::clamp(pitch, m_minPitch, m_maxPitch);
    m_yaw = yaw;
    m_rotation = glm::quat(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), glm::radians(roll)));
}

void Camera::LookAt(const glm::vec3& target, const glm::vec3& worldUp) {
    glm::mat4 lookAtMat = glm::lookAt(m_position, target, worldUp);
    m_rotation = glm::quat_cast(glm::inverse(lookAtMat));
    
    glm::vec3 euler = glm::eulerAngles(m_rotation);
    m_pitch = glm::degrees(euler.x);
    m_yaw = glm::degrees(euler.y);
}

void Camera::Move(const glm::vec3& delta) {
    m_position += delta;
}

void Camera::Move(CameraMovement direction, float deltaTime, float speed) {
    float velocity = speed * deltaTime;
    
    switch (direction) {
        case CameraMovement::Forward:
            m_position += GetForward() * velocity;
            break;
        case CameraMovement::Backward:
            m_position -= GetForward() * velocity;
            break;
        case CameraMovement::Left:
            m_position -= GetRight() * velocity;
            break;
        case CameraMovement::Right:
            m_position += GetRight() * velocity;
            break;
        case CameraMovement::Up:
            m_position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
            break;
        case CameraMovement::Down:
            m_position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
            break;
    }
}

void Camera::Rotate(float pitchDelta, float yawDelta) {
    m_pitch += pitchDelta;
    m_yaw += yawDelta;
    
    // Clamp pitch
    m_pitch = glm::clamp(m_pitch, m_minPitch, m_maxPitch);
    
    // Update rotation quaternion
    m_rotation = glm::quat(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), 0.0f));
}

void Camera::RotateAround(const glm::vec3& target, float pitchDelta, float yawDelta) {
    glm::vec3 offset = m_position - target;
    
    // Rotate offset around Y axis (yaw)
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(yawDelta), glm::vec3(0.0f, 1.0f, 0.0f));
    offset = glm::vec3(yawRotation * glm::vec4(offset, 1.0f));
    
    // Rotate offset around right axis (pitch)
    glm::vec3 right = glm::normalize(glm::cross(offset, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(pitchDelta), right);
    offset = glm::vec3(pitchRotation * glm::vec4(offset, 1.0f));
    
    m_position = target + offset;
    LookAt(target);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + GetForward(), GetUp());
}

glm::mat4 Camera::GetProjectionMatrix() const {
    if (m_type == CameraType::Perspective) {
        return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    } else {
        return glm::ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop, m_nearPlane, m_farPlane);
    }
}

glm::mat4 Camera::GetViewProjectionMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec3 Camera::GetForward() const {
    return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Camera::GetRight() const {
    return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::GetUp() const {
    return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Camera::GetEulerAngles() const {
    return glm::vec3(m_pitch, m_yaw, 0.0f);
}

void Camera::SetFOV(float fov) {
    m_fov = fov;
}

void Camera::SetAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
}

void Camera::SetClipPlanes(float nearPlane, float farPlane) {
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetPitchLimits(float minPitch, float maxPitch) {
    m_minPitch = minPitch;
    m_maxPitch = maxPitch;
    m_pitch = glm::clamp(m_pitch, m_minPitch, m_maxPitch);
}

void Camera::UpdateVectors() {
    // Vectors are calculated on demand from quaternion
}

void Camera::UpdateFrustumPlanes() {
    glm::mat4 vp = GetViewProjectionMatrix();
    
    // Left plane
    m_frustumPlanes[0] = glm::vec4(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0], vp[3][3] + vp[3][0]);
    // Right plane
    m_frustumPlanes[1] = glm::vec4(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0], vp[3][3] - vp[3][0]);
    // Bottom plane
    m_frustumPlanes[2] = glm::vec4(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1], vp[3][3] + vp[3][1]);
    // Top plane
    m_frustumPlanes[3] = glm::vec4(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1], vp[3][3] - vp[3][1]);
    // Near plane
    m_frustumPlanes[4] = glm::vec4(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2], vp[3][3] + vp[3][2]);
    // Far plane
    m_frustumPlanes[5] = glm::vec4(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2], vp[3][3] - vp[3][2]);
    
    // Normalize planes
    for (auto& plane : m_frustumPlanes) {
        float length = glm::length(glm::vec3(plane));
        plane /= length;
    }
}

bool Camera::IsPointInFrustum(const glm::vec3& point) const {
    for (const auto& plane : m_frustumPlanes) {
        if (glm::dot(glm::vec3(plane), point) + plane.w < 0) {
            return false;
        }
    }
    return true;
}

bool Camera::IsSphereInFrustum(const glm::vec3& center, float radius) const {
    for (const auto& plane : m_frustumPlanes) {
        if (glm::dot(glm::vec3(plane), center) + plane.w < -radius) {
            return false;
        }
    }
    return true;
}

bool Camera::IsAABBInFrustum(const glm::vec3& min, const glm::vec3& max) const {
    for (const auto& plane : m_frustumPlanes) {
        glm::vec3 positive = min;
        glm::vec3 negative = max;
        
        if (plane.x >= 0) {
            positive.x = max.x;
            negative.x = min.x;
        }
        if (plane.y >= 0) {
            positive.y = max.y;
            negative.y = min.y;
        }
        if (plane.z >= 0) {
            positive.z = max.z;
            negative.z = min.z;
        }
        
        if (glm::dot(glm::vec3(plane), positive) + plane.w < 0) {
            return false;
        }
    }
    return true;
}

glm::vec3 Camera::ScreenToWorldRay(float screenX, float screenY, float screenWidth, float screenHeight) const {
    // Normalize screen coordinates to [-1, 1]
    float x = (2.0f * screenX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * screenY) / screenHeight;
    
    // Create ray in clip space
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);
    
    // Transform to eye space
    glm::vec4 rayEye = glm::inverse(GetProjectionMatrix()) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    
    // Transform to world space
    glm::vec4 rayWorld = glm::inverse(GetViewMatrix()) * rayEye;
    glm::vec3 rayDir = glm::normalize(glm::vec3(rayWorld));
    
    return rayDir;
}

// FPS Camera Controller
FPSCameraController::FPSCameraController(Camera* camera)
    : m_camera(camera)
    , m_movementSpeed(5.0f)
    , m_mouseSensitivity(0.1f)
    , m_smoothing(0.5f)
    , m_targetPitch(0.0f)
    , m_targetYaw(-90.0f)
    , m_currentPitch(0.0f)
    , m_currentYaw(-90.0f)
    , m_canMove(true)
    , m_canLook(true) {
}

void FPSCameraController::Update(float deltaTime) {
    if (!m_camera) return;
    
    // Smooth rotation
    m_currentPitch = glm::mix(m_currentPitch, m_targetPitch, m_smoothing);
    m_currentYaw = glm::mix(m_currentYaw, m_targetYaw, m_smoothing);
    
    m_camera->SetEulerAngles(m_currentPitch, m_currentYaw);
}

void FPSCameraController::OnMouseMove(float deltaX, float deltaY) {
    if (!m_canLook || !m_camera) return;
    
    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;
    
    m_targetYaw += deltaX;
    m_targetPitch -= deltaY;
    
    // Clamp pitch
    m_targetPitch = glm::clamp(m_targetPitch, -89.0f, 89.0f);
}

void FPSCameraController::OnMouseScroll(float delta) {
    if (!m_camera) return;
    
    float fov = m_camera->GetFOV();
    fov -= delta * 5.0f;
    fov = glm::clamp(fov, 10.0f, 120.0f);
    m_camera->SetFOV(fov);
}

} // namespace fps::renderer
