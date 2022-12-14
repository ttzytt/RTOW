#pragma once
#include <ammintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

#include <cmath>
#include <compare>
#include <iostream>
#include <tuple>

using f8 = double;
using deg = f8;
using rad = f8;

inline f8 rand_f8();
inline f8 rand_f8(f8 mn, f8 mx);
inline i8 rand_i8(i8 mn, f8 mx);
inline i4 rand_i4(i4 mn, i4 mx);

namespace navx2 {
class vec3 {
   public:
	vec3() : e{0, 0, 0} {}
	vec3(f8 x, f8 y, f8 z) : e{x, y, z} {}
	vec3(f8 x) : e{x, x, x} {}

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

	inline vec3 floor() const {
		return vec3(std::floor(e[0]), std::floor(e[1]), std::floor(e[2]));
	}

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

vec3 refract(const vec3 &uv, const vec3 &n, f8 etai_over_etat) {
	return uv.refract(n, etai_over_etat);
}

template <typename T>
inline T lerp(const T &a, const T &b, const T &t) {
	// t 是要预测的值离 a 有多少个 a 到 b 的距离
	// 也就是想要插值的位置离提供的点的距离
	return a + t * (b - a);
}

template <typename T>
inline T lerp2(const T &ld, const T &rd, const T &lu, const T &ru, f8 tx,
			   f8 ty) {
	// left down, right down, left up, right up
	T &&up_mid = lerp(lu, ru, tx);
	T &&dn_mid = lerp(ld, rd, tx);
	return lerp(dn_mid, up_mid, ty);
}

template <typename T>
inline T lerp2(const T &ld, const T &rd, const T &lu, const T &ru,
			   const vec3 &ts) {
	return lerp2(ld, rd, lu, ru, ts[0], ts[1]);
}

template <typename T>
inline T lerp2(const T pts[2][2], const vec3 &ts) {
	// 根据数组的 x，y 值定位
	return lerp2(pts[0][0], pts[1][0], pts[0][1], pts[1][1], ts[0], ts[1]);
}

template <typename T>
inline T lerp3(const T pts[2][2][2], const vec3 &ts) {
	// 值只能放在第四个
	// 组成二维平面的点：
	T &&far_up = lerp(pts[0][1][1], pts[1][1][1], ts[0]);
	// far 代表 z 肯定是 1，up 代表 y 肯定是 1
	T &&far_dn = lerp(pts[0][0][1], pts[1][0][1], ts[0]);
	// far 代表 z 肯定是 1，dn 代表 y 肯定是 0
	T &&clo_up = lerp(pts[0][1][0], pts[1][1][0], ts[0]);
	// clo 代表 z 肯定是 0，up 代表 y 是 1
	T &&clo_dn = lerp(pts[0][0][0], pts[1][0][0], ts[0]);
	// clo 代表 z 是 0，dn 代表 y 是 0
	// https://zhuanlan.zhihu.com/p/77496615
	return lerp2(clo_dn, far_dn, clo_up, far_up, ts.z(), ts.y());
}

using pt3 = vec3;
using color = vec3;	 // 和 typedef 等价
}  // namespace navx2

//--------------------------------------------- avx2

consteval unsigned short shuf_control_rev(const unsigned char p0,
										  const unsigned char p1,
										  const unsigned char p2,
										  const unsigned char p3) {
	// 编译期计算
	// 重排序之后，dst 的 pi 位放到 src 的 i 位
	return (p0 | (p1 << 2) | (p2 << 4) | (p3 << 6));
}

namespace avx2 {
class vec3 {
	// 实际上是 vec4
   public:
	constexpr vec3(const f8 x, const f8 y, const f8 z = 0, const f8 t = 0)
		: vec_data{x, y, z, t} {}
	constexpr vec3(const __m256d &_vec_data) : vec_data(_vec_data) {}
	vec3() : vec_data(_mm256_setzero_pd()) {}
	constexpr vec3(const f8 x) : vec_data{x, x, x, x} {}
	constexpr vec3(const vec3 &x) : vec_data(x.vec_data) {}

	inline f8 &operator[](const int i) const { return vec_data[i]; }
	inline f8 &x() const { return (*this)[0]; }
	inline f8 &y() const { return (*this)[1]; }
	inline f8 &z() const { return (*this)[2]; }
	inline f8 &t() const { return (*this)[3]; }

	inline operator __m256d() { return vec_data; }

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
		static __m256d diff_vec = _mm256_set1_pd(diff);	 // 要不然每次都要重新算
		__m256d ret = _mm256_cmp_pd(vec_data, diff_vec, _CMP_NEQ_OQ);
		// less than, ordered, 如果任何一个是
		// nan 都会返回 false
		// 等于运算会稍快
		return ret[0] == 0 && ret[1] == 0 &&
			   ret[2] == 0;	 // 如果全部都不更大，或者等于的话
	}
	// 下面的需要用到加减法，所以实现在外面

	inline vec3 floor() const { return _mm256_floor_pd(vec_data); }

	inline vec3 ceil() const { return _mm256_ceil_pd(vec_data); }

	static inline vec3 zeros() { return vec3(); }

	inline vec3 permute(char p0, char p1, char p2, char p3) const {
		return _mm256_permute4x64_pd(vec_data, shuf_control_rev(1, 2, 0, 3));
	}

	inline vec3 unit_vec() const;

	inline vec3 reflect(const vec3 &norm) const;

	inline vec3 refract(const vec3 &norm, f8 etain_over_out) const;

	inline vec3 rotate(rad angle, int axis) const;

	inline vec3 rotate(rad sin, rad cos, int axis) const;

   public:
	__m256d vec_data __attribute__((aligned(32)));
};

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return _mm256_add_pd(u.vec_data, v.vec_data);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return _mm256_sub_pd(u.vec_data, v.vec_data);
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

inline vec3 vec3::rotate(f8 sin_the, f8 cos_the, int axis) const {
	// x -> y, y -> z, z -> -x
	switch (axis) {
		case 0:
			return {x(), 
					cos_the * y() - sin_the * z(), 
					sin_the * y() + cos_the * z()};
		case 1:
			return {cos_the * x() + sin_the * z(), 
					y(), 
					-sin_the * x() + cos_the * z()};
		case 2:
			return {cos_the * x() - sin_the * y(), 
					sin_the * x() + cos_the * y(), 
					z()};
	}
}

// 绕哪个轴旋转
inline vec3 vec3::rotate(rad angle, int axis) const {
	return rotate(std::sin(angle), std::cos(angle), axis);
}

inline vec3 vmax(const vec3& a, const vec3& b){
	return _mm256_max_pd(a.vec_data, b.vec_data);
}

inline vec3 vmin(const vec3 &a, const vec3 &b) {
	return _mm256_min_pd(a.vec_data, b.vec_data);
}

template <typename T>
inline T lerp(const T &a, const T &b, const T &t) {
	// t 是要预测的值离 a 有多少个 a 到 b 的距离
	// 也就是想要插值的位置离提供的点的距离
	return a + t * (b - a);
}

template <typename T>
inline T lerp2(const T &ld, const T &rd, const T &lu, const T &ru, const f8 tx,
			   const f8 ty) {
	// left down, right down, left up, right up
	return navx2::lerp2(ld, rd, lu, ru, tx, ty);
}

inline f8 lerp2(const vec3 &all, const vec3 &ts) {
	// all = ld, rd, lu, ru
	// reversed = rd, ld, ru, lu
	vec3 &&reversed =
		_mm256_permute_pd(all.vec_data, shuf_control_rev(1, 0, 3, 2));
	vec3 &&updn_mid = lerp(all, reversed, vec3(ts.x()));
	return lerp(updn_mid[0], updn_mid[2], ts.y());
}

inline f8 lerp2(const vec3 &all, const f8 tx, const f8 ty) {
	return lerp2(all, vec3(tx, ty));
}

template <typename T>
inline T lerp2(const T &ld, const T &rd, const T &lu, const T &ru,
			   const vec3 &ts) {
	return lerp2(ld, rd, lu, ru, ts[0], ts[1]);
}

template <typename T>
inline T lerp2(const T pts[2][2], const vec3 &ts) {
	return lerp2(pts[0][0], pts[1][0], pts[0][1], pts[1][1], ts[0], ts[1]);
}

template <typename T>
inline T lerp3(const T pts[2][2][2], const vec3 &ts) {
	return navx2::lerp3(pts, ts);
}

inline f8 lerp3(const vec3 pts[2], const vec3 &ts) {
	// 两个 vec3 的 ts 分别表示 x 为 0 的点和 x 为 1 的点
	// pts[0]: c000, c001, c010, c011
	// pts[1]: c100, c101, c110, c111
	// mid_square = clo_dn, far_dn, clo_up, far_up
	vec3 &&mid_square = lerp(pts[0], pts[1], vec3(ts.x()));
	return lerp2(mid_square, ts.z(), ts.y());
}

using pt3 = vec3;
using color = vec3;
}  // namespace avx2

// TODO: 之后替换成一个类，avx 继承自这个类？