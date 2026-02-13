#ifndef GLM_GTC_MATRIX_TRANSFORM_HPP
#define GLM_GTC_MATRIX_TRANSFORM_HPP

#include "../glm.hpp"
#include <cmath>

namespace glm {
    template<typename T>
    tmat4x4<T> translate(const tmat4x4<T>& m, const tvec3<T>& v) {
        tmat4x4<T> result(m);
        result[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
        return result;
    }
    
    template<typename T>
    tmat4x4<T> scale(const tmat4x4<T>& m, const tvec3<T>& v) {
        tmat4x4<T> result;
        result[0] = m[0] * v.x;
        result[1] = m[1] * v.y;
        result[2] = m[2] * v.z;
        result[3] = m[3];
        return result;
    }
    
    template<typename T>
    tmat4x4<T> perspective(T fovy, T aspect, T zNear, T zFar) {
        T tanHalfFovy = std::tan(fovy / T(2));
        tmat4x4<T> result(T(0));
        result[0][0] = T(1) / (aspect * tanHalfFovy);
        result[1][1] = T(1) / tanHalfFovy;
        result[2][2] = -(zFar + zNear) / (zFar - zNear);
        result[2][3] = -T(1);
        result[3][2] = -(T(2) * zFar * zNear) / (zFar - zNear);
        return result;
    }
    
    template<typename T>
    tmat4x4<T> lookAt(const tvec3<T>& eye, const tvec3<T>& center, const tvec3<T>& up) {
        tvec3<T> f = normalize(center - eye);
        tvec3<T> s = normalize(cross(f, up));
        tvec3<T> u = cross(s, f);
        
        tmat4x4<T> result(T(1));
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = dot(f, eye);
        return result;
    }
}

#endif
