#ifndef GLM_GTC_TYPE_PTR_HPP
#define GLM_GTC_TYPE_PTR_HPP

#include "../glm.hpp"

namespace glm {

    template<typename T>
    const T* value_ptr(const tvec2<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const tvec3<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const tvec4<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const tmat4x4<T>& m) { return &m[0].x; }

} // namespace glm

#endif // GLM_GTC_TYPE_PTR_HPP
