#pragma once
#include <cmath>
#include <iostream>
using f8 = double;

class vec3 {
   public:
    vec3() : e{0, 0, 0} {}
    vec3(f8 x, f8 y, f8 z) : e{x, y, z} {}

    inline f8 x() const { return e[0]; }
    inline f8 y() const { return e[1]; }
    inline f8 z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    f8 operator[](int i) const { return e[i]; }

    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const f8 t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const f8 t) { return *this *= 1 / t; }

    f8 len() const { return sqrt(len_sq()); }

    f8 len_sq() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline static vec3 rand() { return vec3(rand_f8(), rand_f8(), rand_f8()); }

    inline static vec3 rand(f8 mn, f8 mx) {
        return vec3(rand_f8(mn, mx), rand_f8(mn, mx), rand_f8(mn, mx));
    }

    inline bool near_zero() const {
        const f8 diff = 1e-8;
        return (fabs(e[0]) < diff) && (fabs(e[1]) < diff) &&
               (fabs(e[2]) < diff);
    }

    // 下面的需要用到加减法，所以实现在外面
    inline vec3 unit_vec() const;

    inline vec3 reflect(const vec3 &norm) const;

    inline vec3 refract(const vec3 &norm, f8 etain_over_out) const;

    f8 e[3];
};

// vec3 utils
using namespace std;

inline ostream &operator<<(ostream &out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator/(const vec3 &u, const vec3 &v) {
return vec3(u.e[0] / v.e[0], u.e[1] / v.e[1], u.e[2] / v.e[2]);
}

inline vec3 operator*(f8 t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, f8 t) { return t * v; }

inline vec3 operator/(vec3 v, f8 t) { return (1 / t) * v; }

inline f8 dot(const vec3 &u, const vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline vec3 vec3::unit_vec() const { return *this / this->len(); }
inline vec3 unit_vec(const vec3 &v) { return v.unit_vec(); }

inline vec3 rand_unit_sphere() {
    // 圆心为 (0, 0)
    while (true) {
        vec3 pt = vec3::rand(-1, 1);
        if (pt.len_sq() >= 1) continue;
        return pt;
    }
}

inline vec3 rand_unit_vec() {
    f8 angle = rand_f8(0, 2 * pi);
    f8 z = rand_f8(-1, 1);

    f8 r = sqrt(1 - z * z);  // sqrt(x*x + y*y + z*z) = r;
    // 一个 z 轴确定的圆的半径
    // x^2 + y^2 = 1 - z^2
    return vec3(r * cos(angle), r * sin(angle), z);
}


inline vec3 rand_unit_disk(){
    while(true){
        vec3 tmp(rand_f8(-1, 1), rand_f8(-1, 1), 0);
        if(tmp.len_sq() >= 1) continue;
        return tmp;
    }    
}

inline vec3 vec3::reflect(const vec3 &norm) const {
    // norm 是单位向量
    // this 是入射
    return *this - 2 * dot(*this, norm) * norm;
    // *this · norm 就是 *this 投影到 norm 上的长度乘以 norm 的长度（为 1）
    // 负号是因为两个方向相反，点积会产生负数
}

inline vec3 reflect(const vec3 &v, const vec3 &n) { return v.reflect(n); }

inline vec3 vec3::refract(const vec3 &norm, f8 etain_overout) const {
    // v1 / sin(θ1) = v2 / sin(θ2)
     // n = c / v
    // v = c / n
    // c / n1 / sin(θ1) =  c / n2 / sin(θ2)
    // n1 sin(θ1) = n2 sin(θ2)
    f8 cos_theta_in = fmin(dot(-(*this), norm), 1.0);
    // 当前向量是入射的
    // cos 不可能大于 1

    // 负数时因为方向相反

    vec3 r_out_perp = etain_overout * ((*this) + cos_theta_in * norm);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_sq())) * norm;
    return r_out_parallel + r_out_perp;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    return uv.refract(n, etai_over_etat);
}

using pt3 = vec3;
using color = vec3;  // 和 typedef 等价