#pragma once

#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fps::math {

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Quat = glm::quat;

constexpr float PI = 3.14159265359f;
constexpr float TWO_PI = 6.28318530718f;
constexpr float HALF_PI = 1.57079632679f;
constexpr float EPSILON = 1e-6f;
constexpr float DEG2RAD = PI / 180.0f;
constexpr float RAD2DEG = 180.0f / PI;

inline float ToRadians(float degrees) { return degrees * DEG2RAD; }
inline float ToDegrees(float radians) { return radians * RAD2DEG; }

inline float Clamp(float value, float minVal, float maxVal) {
    return std::max(minVal, std::min(maxVal, value));
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * Clamp(t, 0.0f, 1.0f);
}

inline Vec3 Lerp(const Vec3& a, const Vec3& b, float t) {
    return glm::mix(a, b, Clamp(t, 0.0f, 1.0f));
}

inline float SmoothStep(float edge0, float edge1, float x) {
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

inline float RandomFloat(float minVal = 0.0f, float maxVal = 1.0f) {
    float normalized = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return minVal + normalized * (maxVal - minVal);
}

inline Vec3 RandomVec3(float minVal = -1.0f, float maxVal = 1.0f) {
    return Vec3(
        RandomFloat(minVal, maxVal),
        RandomFloat(minVal, maxVal),
        RandomFloat(minVal, maxVal)
    );
}

inline Vec3 RandomDirection() {
    float theta = RandomFloat(0.0f, TWO_PI);
    float phi = std::acos(RandomFloat(-1.0f, 1.0f));
    return Vec3(
        std::sin(phi) * std::cos(theta),
        std::sin(phi) * std::sin(theta),
        std::cos(phi)
    );
}

inline float Length(const Vec3& v) { return glm::length(v); }
inline Vec3 Normalize(const Vec3& v) { return glm::normalize(v); }
inline float Dot(const Vec3& a, const Vec3& b) { return glm::dot(a, b); }
inline Vec3 Cross(const Vec3& a, const Vec3& b) { return glm::cross(a, b); }
inline float Distance(const Vec3& a, const Vec3& b) { return glm::distance(a, b); }

struct Ray {
    Vec3 origin;
    Vec3 direction;
    
    Ray() = default;
    Ray(const Vec3& orig, const Vec3& dir) : origin(orig), direction(Normalize(dir)) {}
    
    Vec3 At(float t) const { return origin + direction * t; }
};

struct AABB {
    Vec3 min;
    Vec3 max;
    
    AABB() : min(Vec3(0.0f)), max(Vec3(0.0f)) {}
    AABB(const Vec3& minVal, const Vec3& maxVal) : min(minVal), max(maxVal) {}
    
    Vec3 GetCenter() const { return (min + max) * 0.5f; }
    Vec3 GetExtents() const { return (max - min) * 0.5f; }
    
    bool Contains(const Vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
    
    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    
    bool IntersectsRay(const Ray& ray, float& tMin, float& tMax) const {
        Vec3 invDir = Vec3(1.0f) / ray.direction;
        Vec3 t1 = (min - ray.origin) * invDir;
        Vec3 t2 = (max - ray.origin) * invDir;
        
        Vec3 tNear = glm::min(t1, t2);
        Vec3 tFar = glm::max(t1, t2);
        
        tMin = std::max(std::max(tNear.x, tNear.y), tNear.z);
        tMax = std::min(std::min(tFar.x, tFar.y), tFar.z);
        
        return tMin <= tMax && tMax >= 0.0f;
    }
    
    void Expand(const Vec3& point) {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }
    
    void Expand(const AABB& other) {
        min = glm::min(min, other.min);
        max = glm::max(max, other.max);
    }
};

struct Sphere {
    Vec3 center;
    float radius;
    
    Sphere() : center(Vec3(0.0f)), radius(0.0f) {}
    Sphere(const Vec3& c, float r) : center(c), radius(r) {}
    
    bool Contains(const Vec3& point) const {
        return Distance(center, point) <= radius;
    }
    
    bool Intersects(const Sphere& other) const {
        float distSq = glm::distance2(center, other.center);
        float radiusSum = radius + other.radius;
        return distSq <= radiusSum * radiusSum;
    }
    
    bool IntersectsRay(const Ray& ray, float& t) const {
        Vec3 oc = ray.origin - center;
        float a = Dot(ray.direction, ray.direction);
        float b = 2.0f * Dot(oc, ray.direction);
        float c = Dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        
        if (discriminant < 0.0f) return false;
        
        t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (t < 0.0f) t = (-b + std::sqrt(discriminant)) / (2.0f * a);
        
        return t >= 0.0f;
    }
};

class Transform {
public:
    Vec3 position{0.0f};
    Quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3 scale{1.0f};
    
    Transform() = default;
    Transform(const Vec3& pos, const Quat& rot = Quat(1.0f, 0.0f, 0.0f, 0.0f), const Vec3& scl = Vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}
    
    Mat4 GetMatrix() const {
        Mat4 mat = glm::translate(Mat4(1.0f), position);
        mat *= glm::mat4_cast(rotation);
        mat = glm::scale(mat, scale);
        return mat;
    }
    
    Vec3 GetForward() const { return rotation * Vec3(0.0f, 0.0f, -1.0f); }
    Vec3 GetRight() const { return rotation * Vec3(1.0f, 0.0f, 0.0f); }
    Vec3 GetUp() const { return rotation * Vec3(0.0f, 1.0f, 0.0f); }
    
    void SetEulerAngles(float pitch, float yaw, float roll = 0.0f) {
        rotation = glm::quat(Vec3(pitch, yaw, roll));
    }
    
    Vec3 GetEulerAngles() const {
        return glm::eulerAngles(rotation);
    }
    
    void Rotate(float angle, const Vec3& axis) {
        rotation = glm::rotate(rotation, angle, axis);
    }
    
    void LookAt(const Vec3& target, const Vec3& worldUp = Vec3(0.0f, 1.0f, 0.0f)) {
        Mat4 lookAtMat = glm::lookAt(position, target, worldUp);
        rotation = glm::quat_cast(glm::inverse(lookAtMat));
    }
};

} // namespace fps::math
