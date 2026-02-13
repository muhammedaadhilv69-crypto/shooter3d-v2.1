#!/bin/bash
# GLM is header-only, we'll create minimal required headers

mkdir -p external/glm/glm/gtc
mkdir -p external/glm/glm/gtx

# Create main glm.hpp
cat > external/glm/glm/glm.hpp << 'GLMEOF'
// Minimal GLM header for FPS game
#ifndef GLM_GLM_HPP
#define GLM_GLM_HPP

#include <cmath>
#include <algorithm>

namespace glm {
    // Vector types
    template<typename T>
    struct tvec2 {
        T x, y;
        tvec2() : x(0), y(0) {}
        tvec2(T s) : x(s), y(s) {}
        tvec2(T x, T y) : x(x), y(y) {}
    };
    
    template<typename T>
    struct tvec3 {
        T x, y, z;
        tvec3() : x(0), y(0), z(0) {}
        tvec3(T s) : x(s), y(s), z(s) {}
        tvec3(T x, T y, T z) : x(x), y(y), z(z) {}
        
        tvec3 operator+(const tvec3& v) const { return tvec3(x+v.x, y+v.y, z+v.z); }
        tvec3 operator-(const tvec3& v) const { return tvec3(x-v.x, y-v.y, z-v.z); }
        tvec3 operator*(T s) const { return tvec3(x*s, y*s, z*s); }
        tvec3 operator*(const tvec3& v) const { return tvec3(x*v.x, y*v.y, z*v.z); }
        tvec3& operator+=(const tvec3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
        tvec3& operator-=(const tvec3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
        tvec3& operator*=(T s) { x*=s; y*=s; z*=s; return *this; }
        tvec3 operator-() const { return tvec3(-x, -y, -z); }
    };
    
    template<typename T>
    struct tvec4 {
        T x, y, z, w;
        tvec4() : x(0), y(0), z(0), w(0) {}
        tvec4(T s) : x(s), y(s), z(s), w(s) {}
        tvec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        tvec4(const tvec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
    };
    
    // Matrix type (column-major)
    template<typename T>
    struct tmat4x4 {
        tvec4<T> value[4];
        
        tmat4x4() {
            value[0] = tvec4<T>(1, 0, 0, 0);
            value[1] = tvec4<T>(0, 1, 0, 0);
            value[2] = tvec4<T>(0, 0, 1, 0);
            value[3] = tvec4<T>(0, 0, 0, 1);
        }
        
        tmat4x4(T s) {
            value[0] = tvec4<T>(s, 0, 0, 0);
            value[1] = tvec4<T>(0, s, 0, 0);
            value[2] = tvec4<T>(0, 0, s, 0);
            value[3] = tvec4<T>(0, 0, 0, s);
        }
        
        tvec4<T>& operator[](int i) { return value[i]; }
        const tvec4<T>& operator[](int i) const { return value[i]; }
    };
    
    typedef tvec2<float> vec2;
    typedef tvec3<float> vec3;
    typedef tvec4<float> vec4;
    typedef tmat4x4<float> mat4;
    
    // Vector operations
    template<typename T>
    T dot(const tvec3<T>& a, const tvec3<T>& b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    
    template<typename T>
    tvec3<T> cross(const tvec3<T>& a, const tvec3<T>& b) {
        return tvec3<T>(
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        );
    }
    
    template<typename T>
    T length(const tvec3<T>& v) {
        return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }
    
    template<typename T>
    tvec3<T> normalize(const tvec3<T>& v) {
        T len = length(v);
        if (len > 0) return v * (T(1) / len);
        return v;
    }
    
    template<typename T>
    T radians(T degrees) {
        return degrees * T(0.01745329251994329576923690768489);
    }
}

#endif
GLMEOF

echo "GLM headers created successfully"
