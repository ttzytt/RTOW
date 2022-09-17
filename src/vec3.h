#pragma once

#define __AVX__ 1
#define __AVX2__ 1
#define __SSE__ 1
#define __SSE2__ 1
#define __SSE2_MATH__ 1
#define __SSE3__ 1
#define __SSE4_1__ 1
#define __SSE4_2__ 1
#define __SSE_MATH__ 1
#define __SSSE3__ 1
#pragma GCC target( \
    "sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")

#include <emmintrin.h>
#include <immintrin.h>

#include <cmath>
#include <compare>
#include <iostream>
#include <tuple>

using f8 = double;

inline f8 rand_f8();
inline f8 rand_f8(f8 mn, f8 mx);
inline i8 rand_i8(i8 mn, f8 mx);
inline i4 rand_i4(i4 mn, i4 mx);

namespace navx2 {
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

    auto operator<=>(const vec3 &) const = default;

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

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

// 基本运算

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

// 随机生成函数

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

inline vec3 rand_unit_disk() {
    while (true) {
        vec3 tmp(rand_f8(-1, 1), rand_f8(-1, 1), 0);
        if (tmp.len_sq() >= 1) continue;
        return tmp;
    }
}

// 光学相关

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

vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    return uv.refract(n, etai_over_etat);
}

vec3 lerp(const vec3 &a, const vec3 &b, f8 t) {
    // 返回的 y 就是插值结果
    return a + t * (b - a);
}

inline vec3 lerp2(const vec3 &ld, const vec3 &rd, const vec3 &lu,
                  const vec3 &ru, f8 tx, f8 ty) {
    // left down, right down, left up, right up
    // tx 和 ty 分别表示想要插的值点和 ld 的 x，y 距离
    // 每个点的坐标存在 vec3 的前两位，值存在第 3 或 4 位
    // 提供的四个点里只能有两种 x 和 y 坐标
    vec3&& up_mid = lerp(lu, ru, tx);
    vec3&& dn_mid = lerp(ld, rd, tx);
    return lerp(up_mid, dn_mid, ty);
}

inline vec3 lerp2(const vec3 &ld, const vec3 &rd, const vec3 &lu,
                  const vec3 &ru, const vec3& ts){
    return lerp2(ld, rd, lu, ru, ts[0], ts[1]);
}

inline vec3 lerp2(const vec3 pts[2][2], const vec3 &ts){
    // 根据数组的 x，y 值定位
    return lerp2(pts[0][0], pts[1][0], pts[0][1], pts[1][1], ts[0], ts[1]);
}

// 只能存三个数所以搞不了 lerp3

using pt3 = vec3;
using color = vec3;  // 和 typedef 等价
}  // namespace navx2

//--------------------------------------------- avx2

namespace avx2 {
class vec3 {
    // 实际上是 vec4
   public:
    vec3(f8 x, f8 y, f8 z, f8 t = 0) { vec_data = _mm256_set_pd(t, z, y, x); }

    vec3(const __m256d &_vec_data) : vec_data(_vec_data) {}

    vec3() : vec_data(_mm256_setzero_pd()) {}

    vec3(const navx2::vec3 &v) : vec_data(_mm256_set_pd(0, v[2], v[1], v[0])) {}

    inline f8 operator[](int i) const { return vec_data[i]; }
    inline f8 x() const { return (*this)[0]; }
    inline f8 y() const { return (*this)[1]; }
    inline f8 z() const { return (*this)[2]; }

    inline vec3 operator-() const {
        return _mm256_sub_pd(_mm256_setzero_pd(), vec_data);
        // 0 - vec_data
    }

    inline vec3 &operator+=(const vec3 &v) {
        vec_data = _mm256_add_pd(vec_data, v.vec_data);
        return *this;
    }

    inline vec3 &operator-=(const vec3 &v) {
        vec_data = _mm256_sub_pd(vec_data, v.vec_data);
        return *this;
    }

    inline vec3 &operator*=(const vec3 &v) {
        vec_data = _mm256_mul_pd(vec_data, v.vec_data);
        return *this;
    }

    inline vec3 &operator*=(f8 t) {
        *this *= _mm256_set1_pd(t);
        return *this;
    }

    inline vec3 &operator/=(f8 t) { return *this *= (1 / t); }

    inline f8 len_sq() const {
        __m256d tmp = _mm256_mul_pd(vec_data, vec_data);
        return tmp[0] + tmp[1] + tmp[2];
    }

    inline f8 len() const { return sqrt(len_sq()); }

    inline static vec3 rand() { return vec3(rand_f8(), rand_f8(), rand_f8()); }

    inline static vec3 rand(f8 mn, f8 mx) {
        return vec3(rand_f8(mn, mx), rand_f8(mn, mx), rand_f8(mn, mx));
    }

    inline bool near_zero() const {
        const f8 diff = 1e-8;
        static __m256d diff_vec = _mm256_set1_pd(diff);  // 要不然每次都要重新算
        __m256d ret = _mm256_cmp_pd(vec_data, diff_vec, _CMP_NEQ_OQ);
        // less than, ordered, 如果任何一个是
        // nan 都会返回 false
        // 等于运算会稍快
        return ret[0] == 0 && ret[1] == 0 &&
               ret[2] == 0;  // 如果全部都不更大，或者等于的话
    }
    // 下面的需要用到加减法，所以实现在外面
    inline vec3 unit_vec() const;

    inline vec3 reflect(const vec3 &norm) const;

    inline vec3 refract(const vec3 &norm, f8 etain_over_out) const;

   public:
    __m256d vec_data __attribute__((aligned(32)));
};

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(_mm256_add_pd(u.vec_data, v.vec_data));
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(_mm256_sub_pd(u.vec_data, v.vec_data));
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return _mm256_mul_pd(u.vec_data, v.vec_data);
}

inline vec3 operator/(const vec3 &u, const vec3 &v) {
    return _mm256_div_pd(u.vec_data, v.vec_data);
}

inline vec3 operator*(f8 t, const vec3 &v) {
    return _mm256_mul_pd(_mm256_set1_pd(t), v.vec_data);
}

inline vec3 operator*(const vec3 &v, f8 t) { return t * v; }

inline vec3 operator/(vec3 v, f8 t) { return (1 / t) * v; }

inline f8 dot(const vec3 &u, const vec3 &v) {
    vec3 &&tmp = u * v;
    return tmp[0] + tmp[1] + tmp[2];
}

consteval unsigned short shuf_control_rev(const unsigned char p0,
                                          const unsigned char p1,
                                          const unsigned char p2,
                                          const unsigned char p3) {
    // 编译期计算
    // 重排序之后，src 的第 i 位放到 dst 的第 pi 位
    return (p0 | (p1 << 2) | (p2 << 4) | (p3 << 6));
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    /*
    t1 = u1, u2, u0
    t2 = v2, v0, v1
    t3 = t1 * t2

    k1 = u2, u0, u1
    k2 = v1, v2, v0
    k3 = k1 * k2

    ret = t3 - k3
     */

    /*
    DEFINE SELECT4(src, control) {
        CASE(control[1:0]) OF
        0:	tmp[63:0] := src[63:0]
        1:	tmp[63:0] := src[127:64]
        2:	tmp[63:0] := src[191:128]
        3:	tmp[63:0] := src[255:192]
        ESAC
        RETURN tmp[63:0]
    }
    dst[63:0] := SELECT4(a[255:0], imm8[1:0])
    dst[127:64] := SELECT4(a[255:0], imm8[3:2])
    dst[191:128] := SELECT4(a[255:0], imm8[5:4])
    dst[255:192] := SELECT4(a[255:0], imm8[7:6])
    dst[MAX:256] := 0
     */

    __m256d &&t1 =
        _mm256_permute4x64_pd(u.vec_data, shuf_control_rev(1, 2, 0, 3));
    __m256d &&t2 =
        _mm256_permute4x64_pd(v.vec_data, shuf_control_rev(2, 0, 1, 3));
    __m256d &&t3 = _mm256_mul_pd(t1, t2);

    __m256d &&k1 =
        _mm256_permute4x64_pd(u.vec_data, shuf_control_rev(2, 0, 1, 3));
    __m256d &&k2 =
        _mm256_permute4x64_pd(v.vec_data, shuf_control_rev(1, 2, 0, 3));
    __m256d &&k3 = _mm256_mul_pd(k1, k2);

    return _mm256_sub_pd(t3, k3);
}

inline vec3 vec3::unit_vec() const { return *this / this->len(); }
inline vec3 unit_vec(const vec3 &v) { return v.unit_vec(); }

inline vec3 vec3::reflect(const vec3 &norm) const {
    return *this - 2 * dot(*this, norm) * norm;
}
inline vec3 reflect(const vec3 &v, const vec3 &n) { return v.reflect(n); }

inline vec3 vec3::refract(const vec3 &norm, f8 etain_overout) const {
    f8 cos_theta_in = fmin(dot(-(*this), norm), 1.0);
    vec3 r_out_perp = etain_overout * ((*this) + cos_theta_in * norm);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_sq())) * norm;
    return r_out_parallel + r_out_perp;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    return uv.refract(n, etai_over_etat);
}

inline vec3 lerp(const vec3 &a, const vec3 &b, f8 t) {
    // 第 2, 3, 4 （选一个）位应该储存 a 和 b 的值
    // t 是离 a 的 x 坐标距离
    return a + t * (b - a);
}

inline vec3 lerp2(const vec3 &ld, const vec3 &rd, const vec3 &lu,
                  const vec3 &ru, f8 tx, f8 ty) {
    // left down, right down, left up, right up
    // tx 和 ty 分别表示想要插的值点和 ld 的 x，y 距离
    // 每个点的坐标存在 vec3 的前两位，值存在第 3 或 4 位
    // 提供的四个点里只能有两种 x 和 y 坐标
    vec3&& up_mid = lerp(lu, ru, tx);
    vec3&& dn_mid = lerp(ld, rd, tx);
    return lerp(up_mid, dn_mid, ty);
}

inline vec3 lerp2(const vec3 &ld, const vec3 &rd, const vec3 &lu,
                  const vec3 &ru, const vec3& ts){
    return lerp2(ld, rd, lu, ru, ts[0], ts[1]);
}

inline vec3 lerp2(const vec3 pts[2][2], const vec3 &ts){
    // 根据数组的 x，y 值定位
    return lerp2(pts[0][0], pts[1][0], pts[0][1], pts[1][1], ts[0], ts[1]);
}

inline vec3 lerp3(const vec3 pts[2][2][2], const vec3 &ts){
    // 值只能放在第四个
    // 组成二维平面的点：
    
    vec3&& far_up = lerp(pts[0][1][1], pts[1][1][1], ts[0]); 
    // far 代表 z 肯定是 1，up 代表 y 肯定是 1 
    vec3&& far_dn = lerp(pts[0][0][1], pts[1][0][1], ts[0]);
    // far 代表 z 肯定是 1，dn 代表 y 肯定是 0
    vec3&& clo_up = lerp(pts[0][1][0], pts[1][1][0], ts[0]);
    // clo 代表 z 肯定是 0，up 代表 y 是 1
    vec3&& clo_dn = lerp(pts[0][0][0], pts[1][0][0], ts[0]);
    // clo 代表 z 是 0，dn 代表 y 是 0
    // https://zhuanlan.zhihu.com/p/77496615
    return lerp2(clo_dn, far_dn, clo_up, far_up, ts.z(), ts.x());
}
using pt3 = vec3;
using color = vec3;

}  // namespace avx2

// TODO: 之后替换成一个类，avx 继承自这个类？