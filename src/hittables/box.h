#pragma once
#include <vector>

#include "../aabb.h"
#include "../rtow.h"
#include "bvh.h"
#include "hittable_list.h"
#include "rect_slice.h"

class box : public hittable {
   public:
	box() = default;
	box(const pt3& _mn, const pt3& _mx, const shared_ptr<material>& mats);
	box(const pt3& _mn, const pt3& _mx,
		const std::array<shared_ptr<material>, 6>& mats);

	std::optional<hit_rec> hit(const ray& r, f8 t_min,
							   f8 t_max) const override {
		return faces.hit(r, t_min, t_max);
	}
	std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override {
		return aabb(mn, mx);
	}
	pt3 mn, mx;
	bvh_node faces;
};

box::box(const pt3& _mn, const pt3& _mx,
		const std::array<shared_ptr<material>, 6>& mats)
	: mn(_mn), mx(_mx) {
	hittable_list temp;

	if (mats[0] != nullptr)
		temp.add(make_shared<rect_slice<0>>(mn[1], mx[1], mn[2], mx[2], mn[0],
											mats[0]));	// 左
	if (mats[1] != nullptr)
		temp.add(make_shared<rect_slice<0>>(mn[1], mx[1], mn[2], mx[2], mx[0],
											mats[1]));	// 右

	if (mats[2] != nullptr)
		temp.add(make_shared<rect_slice<1>>(mn[0], mx[0], mn[2], mx[2], mx[1],
											mats[2]));	// 上
	if (mats[3] != nullptr)
		temp.add(make_shared<rect_slice<1>>(mn[0], mx[0], mn[2], mx[2], mn[1],
											mats[3]));	// 下
	if (mats[4] != nullptr)
		temp.add(make_shared<rect_slice<2>>(mn[0], mx[0], mn[1], mx[1], mn[2],
											mats[4]));	// 前
	if (mats[5] != nullptr)
		temp.add(make_shared<rect_slice<2>>(mn[0], mx[0], mn[1], mx[1], mx[2],
											mats[5]));	// 后
	faces = bvh_node(temp);
}

box::box(const pt3& _mn, const pt3& _mx, const shared_ptr<material>& mats) {
	// mats 顺序：左右，上下，前后
    std::array<shared_ptr<material>, 6> arr;
    arr.fill(mats);
	box(_mn, _mx, arr);
}