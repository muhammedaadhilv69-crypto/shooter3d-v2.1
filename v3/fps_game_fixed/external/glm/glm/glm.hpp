// Minimal GLM implementation for FPS game - with bug fixes
#ifndef GLM_GLM_HPP
#define GLM_GLM_HPP

#include <cmath>
#include <algorithm>

namespace glm {

    // ---- vec2 ----
    template<typename T>
    struct tvec2 {
        T x, y;
        tvec2() : x(0), y(0) {}
        tvec2(T s) : x(s), y(s) {}
        tvec2(T x, T y) : x(x), y(y) {}
        T& operator[](int i) { return (&x)[i]; }
        const T& operator[](int i) const { return (&x)[i]; }
    };

    // ---- vec3 (with r/g/b aliases) ----
    template<typename T>
    struct tvec3 {
        union { T x; T r; };
        union { T y; T g; };
        union { T z; T b; };

        tvec3() : x(0), y(0), z(0) {}
        tvec3(T s) : x(s), y(s), z(s) {}
        tvec3(T x, T y, T z) : x(x), y(y), z(z) {}

        tvec3 operator+(const tvec3& v) const { return tvec3(x+v.x, y+v.y, z+v.z); }
        tvec3 operator-(const tvec3& v) const { return tvec3(x-v.x, y-v.y, z-v.z); }
        tvec3 operator*(T s) const { return tvec3(x*s, y*s, z*s); }
        tvec3 operator*(const tvec3& v) const { return tvec3(x*v.x, y*v.y, z*v.z); }
        tvec3 operator/(T s) const { return tvec3(x/s, y/s, z/s); }
        tvec3& operator+=(const tvec3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
        tvec3& operator-=(const tvec3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
        tvec3& operator*=(T s) { x*=s; y*=s; z*=s; return *this; }
        tvec3& operator*=(const tvec3& v) { x*=v.x; y*=v.y; z*=v.z; return *this; }
        tvec3 operator-() const { return tvec3(-x, -y, -z); }
        bool operator==(const tvec3& v) const { return x==v.x && y==v.y && z==v.z; }
        T& operator[](int i) { return (&x)[i]; }
        const T& operator[](int i) const { return (&x)[i]; }
    };

    template<typename T>
    tvec3<T> operator*(T s, const tvec3<T>& v) { return v * s; }

    // ---- vec4 (with r/g/b/a aliases) ----
    template<typename T>
    struct tvec4 {
        union { T x; T r; };
        union { T y; T g; };
        union { T z; T b; };
        union { T w; T a; };

        tvec4() : x(0), y(0), z(0), w(0) {}
        tvec4(T s) : x(s), y(s), z(s), w(s) {}
        tvec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        tvec4(const tvec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

        tvec4 operator+(const tvec4& v) const { return tvec4(x+v.x, y+v.y, z+v.z, w+v.w); }
        tvec4 operator*(T s) const { return tvec4(x*s, y*s, z*s, w*s); }
        T& operator[](int i) { return (&x)[i]; }
        const T& operator[](int i) const { return (&x)[i]; }
    };

    // ---- mat4 ----
    template<typename T>
    struct tmat4x4 {
        tvec4<T> value[4];

        tmat4x4() {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    value[i][j] = (i == j) ? T(1) : T(0);
        }
        tmat4x4(T s) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    value[i][j] = (i == j) ? s : T(0);
        }

        tvec4<T>& operator[](int i) { return value[i]; }
        const tvec4<T>& operator[](int i) const { return value[i]; }

        tmat4x4 operator*(const tmat4x4& m) const {
            tmat4x4 result(T(0));
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        result[i][j] += value[k][j] * m[i][k];
            return result;
        }

        tvec4<T> operator*(const tvec4<T>& v) const {
            tvec4<T> result;
            for (int i = 0; i < 4; ++i) {
                result[i] = T(0);
                for (int j = 0; j < 4; ++j)
                    result[i] += value[j][i] * v[j];
            }
            return result;
        }
    };

    // Type aliases
    typedef tvec2<float>  vec2;
    typedef tvec3<float>  vec3;
    typedef tvec4<float>  vec4;
    typedef tvec2<int>    ivec2;
    typedef tvec3<int>    ivec3;
    typedef tmat4x4<float> mat4;

    // ---- Math functions ----
    template<typename T>
    T radians(T degrees) { return degrees * T(3.14159265358979323846) / T(180); }

    template<typename T>
    T length(const tvec3<T>& v) {
        return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    template<typename T>
    tvec3<T> normalize(const tvec3<T>& v) {
        T len = length(v);
        if (len < T(1e-8)) return tvec3<T>(0);
        return v / len;
    }

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
    tmat4x4<T> lookAt(const tvec3<T>& eye, const tvec3<T>& center, const tvec3<T>& up) {
        tvec3<T> f = normalize(center - eye);
        tvec3<T> s = normalize(cross(f, up));
        tvec3<T> u = cross(s, f);

        tmat4x4<T> result(T(1));
        result[0][0] = s.x;  result[1][0] = s.y;  result[2][0] = s.z;
        result[0][1] = u.x;  result[1][1] = u.y;  result[2][1] = u.z;
        result[0][2] = -f.x; result[1][2] = -f.y; result[2][2] = -f.z;
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] =  dot(f, eye);
        return result;
    }

    template<typename T>
    tmat4x4<T> transpose(const tmat4x4<T>& m) {
        tmat4x4<T> result(T(0));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result[i][j] = m[j][i];
        return result;
    }

    template<typename T>
    tmat4x4<T> inverse(const tmat4x4<T>& m) {
        // Compute inverse using cofactor expansion
        T Coef00 = m[2][2]*m[3][3] - m[3][2]*m[2][3];
        T Coef02 = m[1][2]*m[3][3] - m[3][2]*m[1][3];
        T Coef03 = m[1][2]*m[2][3] - m[2][2]*m[1][3];
        T Coef04 = m[2][1]*m[3][3] - m[3][1]*m[2][3];
        T Coef06 = m[1][1]*m[3][3] - m[3][1]*m[1][3];
        T Coef07 = m[1][1]*m[2][3] - m[2][1]*m[1][3];
        T Coef08 = m[2][1]*m[3][2] - m[3][1]*m[2][2];
        T Coef10 = m[1][1]*m[3][2] - m[3][1]*m[1][2];
        T Coef11 = m[1][1]*m[2][2] - m[2][1]*m[1][2];
        T Coef12 = m[2][0]*m[3][3] - m[3][0]*m[2][3];
        T Coef14 = m[1][0]*m[3][3] - m[3][0]*m[1][3];
        T Coef15 = m[1][0]*m[2][3] - m[2][0]*m[1][3];
        T Coef16 = m[2][0]*m[3][2] - m[3][0]*m[2][2];
        T Coef18 = m[1][0]*m[3][2] - m[3][0]*m[1][2];
        T Coef19 = m[1][0]*m[2][2] - m[2][0]*m[1][2];
        T Coef20 = m[2][0]*m[3][1] - m[3][0]*m[2][1];
        T Coef22 = m[1][0]*m[3][1] - m[3][0]*m[1][1];
        T Coef23 = m[1][0]*m[2][1] - m[2][0]*m[1][1];

        tvec4<T> Fac0(Coef00, Coef00, Coef02, Coef03);
        tvec4<T> Fac1(Coef04, Coef04, Coef06, Coef07);
        tvec4<T> Fac2(Coef08, Coef08, Coef10, Coef11);
        tvec4<T> Fac3(Coef12, Coef12, Coef14, Coef15);
        tvec4<T> Fac4(Coef16, Coef16, Coef18, Coef19);
        tvec4<T> Fac5(Coef20, Coef20, Coef22, Coef23);

        tvec4<T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
        tvec4<T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
        tvec4<T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
        tvec4<T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

        tvec4<T> Inv0(Vec1*Fac0 - Vec2*Fac1 + Vec3*Fac2);
        tvec4<T> Inv1(Vec0*Fac0 - Vec2*Fac3 + Vec3*Fac4);
        tvec4<T> Inv2(Vec0*Fac1 - Vec1*Fac3 + Vec3*Fac5);
        tvec4<T> Inv3(Vec0*Fac2 - Vec1*Fac4 + Vec2*Fac5);

        tvec4<T> SignA(+1, -1, +1, -1);
        tvec4<T> SignB(-1, +1, -1, +1);
        tmat4x4<T> Inverse;
        for (int i = 0; i < 4; ++i) {
            Inverse[0][i] = Inv0[i] * SignA[i];
            Inverse[1][i] = Inv1[i] * SignB[i];
            Inverse[2][i] = Inv2[i] * SignA[i];
            Inverse[3][i] = Inv3[i] * SignB[i];
        }

        tvec4<T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);
        tvec4<T> Dot0(m[0] * Row0);
        T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);
        T OneOverDeterminant = T(1) / Dot1;

        tmat4x4<T> result(T(0));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result[i][j] = Inverse[i][j] * OneOverDeterminant;
        return result;
    }

} // namespace glm

#endif // GLM_GLM_HPP
