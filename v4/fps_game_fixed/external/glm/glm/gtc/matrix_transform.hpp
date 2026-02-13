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
        tmat4x4<T> result(T(0));
        result[0] = m[0] * v.x;
        result[1] = m[1] * v.y;
        result[2] = m[2] * v.z;
        result[3] = m[3];
        return result;
    }

    template<typename T>
    tmat4x4<T> rotate(const tmat4x4<T>& m, T angle, const tvec3<T>& axis) {
        T c = std::cos(angle);
        T s = std::sin(angle);
        tvec3<T> a = normalize(axis);
        tvec3<T> temp = a * (T(1) - c);

        tmat4x4<T> rot(T(0));
        rot[0][0] = c + temp.x * a.x;
        rot[0][1] = temp.x * a.y + s * a.z;
        rot[0][2] = temp.x * a.z - s * a.y;
        rot[1][0] = temp.y * a.x - s * a.z;
        rot[1][1] = c + temp.y * a.y;
        rot[1][2] = temp.y * a.z + s * a.x;
        rot[2][0] = temp.z * a.x + s * a.y;
        rot[2][1] = temp.z * a.y - s * a.x;
        rot[2][2] = c + temp.z * a.z;

        tmat4x4<T> result(T(0));
        result[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
        result[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
        result[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
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
    tmat4x4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
        tmat4x4<T> result(T(1));
        result[0][0] =  T(2) / (right - left);
        result[1][1] =  T(2) / (top - bottom);
        result[2][2] = -T(2) / (zFar - zNear);
        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
        result[3][2] = -(zFar + zNear) / (zFar - zNear);
        return result;
    }

} // namespace glm

#endif // GLM_GTC_MATRIX_TRANSFORM_HPP
