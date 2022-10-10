#include <optional>
#include "../aabb.h"
#include "hittable.h"
#include "../rtow.h"
#pragma once

constexpr inline pt3 g_pt_by_axis(const f8 a, const f8 b, const f8 c, const int axis) {
	// cosnteval 只能编译期运行，constexpr 不是
	switch (axis) {
		case 0:
			return pt3{c, a, b};
		case 1:
			return pt3{a, c, b};
		case 2:
			return pt3{a, b, c};
	}
	return pt3{0, 0, 0};
}

constexpr std::pair<int, int> g_other_axis_id(const int axis) {
	// 给定没有长度的那个轴，根据编号获取另外两个轴的 id
	switch (axis) {
		case 0:
			return {1, 2};
		case 1:
			return {0, 2};
		case 2:
			return {0, 1};
	}
}

template <int AXIS>
class rect_slice : public hittable {
   public:
	constexpr rect_slice(const f8 _a00, const f8 _a01, const f8 _a10,
						 const f8 _a11, const f8 _a2,
						 shared_ptr<material> _mat_ptr)
		: a00(_a00),
		  a01(_a01),
		  a10(_a10),
		  a11(_a11),
		  a2(_a2),
		  mat_ptr(_mat_ptr) {}

	std::optional<hit_rec> hit(const ray& r, f8 t_min,
							   f8 t_max) const override {
		f8 t = (a2 - r.orig[AXIS]) / r.dir[AXIS];
		/*
			pz(t) = az + tbz
			t = pz(t) - az / bz, pz(t) is a2
		 */
		if (t < t_min || t > t_max) return std::nullopt;
		auto&& [a0id, a1id] = g_other_axis_id(AXIS);
		f8 a0 = r.orig[a0id] + t * r.dir[a0id];
		f8 a1 = r.orig[a1id] + t * r.dir[a1id];
		if (a0 < a00 || a0 > a01 || a1 < a10 || a1 > a11) return std::nullopt;
		hit_rec rec;
		rec.azim = (a0 - a00) / (a01 - a00);  // 把击中点映射到 [0, 1] 的范围中
		rec.polar = (a1 - a01) / (a11 - a10);
		rec.t = t;
		rec.hit_pt = r.at(t);
		vec3 norm;
		norm[AXIS] = 1;
		rec.norm = norm;
		rec.set_face_normal(r, norm);
		rec.mat_ptr = mat_ptr;
		return rec;
	}

	std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override {
		return aabb(g_pt_by_axis(a00, a10, a2 - 0.0001, AXIS),
					g_pt_by_axis(a01, a11, a2 + 0.0001, AXIS));
	}

	f8 a00, a01, a10, a11, a2;
	shared_ptr<material> mat_ptr;
	// 长方体在 a2 轴上没有长度
	// a0, a1 按照 xyz 的顺序，如 a2 是 y，a0 a1 分别是 xz
};

// template <>
// class rect_slice<-1> : public hittable {
// 	// 运行时确定轴向

// };