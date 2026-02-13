#pragma once
#include "../glm.hpp"
namespace glm {
    template<typename T>
    T length2(const tvec3<T>& v) { return v.x*v.x + v.y*v.y + v.z*v.z; }
}
