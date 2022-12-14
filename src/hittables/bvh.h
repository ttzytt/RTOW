#pragma once
#include "../aabb.h"
#include "../rtow.h"
#include <functional>
#include "hittable.h"

// bouding box hierarchy

class bvh_node : public hittable {
   public:
	bvh_node() = default;
	bvh_node(const hittable_list& list, f8 tm0 = 0, f8 tm1 = infinity)
		: bvh_node(list.objs, 0, list.objs.size(), tm0, tm1) {}

	bvh_node(const std::vector<std::shared_ptr<hittable>>& objs, int beg,
			 int ed, f8 tm0, f8 tm1);

	virtual std::optional<hit_rec> hit(const ray& r, f8 t_min,
									   f8 t_max) const override;
	virtual std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override {
		return box;
	}  // 一个时间段内的撞击盒

	shared_ptr<hittable> ch[2];
	aabb box;
};

std::optional<hit_rec> bvh_node::hit(const ray& r, f8 t_min, f8 t_max) const {
	if (!box.hit(r, t_min, t_max)) return std::nullopt;

	auto&& left_rec = ch[0]->hit(r, t_min, t_max);
	auto&& right_rec =
		ch[1]->hit(r, t_min, left_rec.has_value() ? left_rec->t : t_max);
	// 找的是第一个击中记录

	if (right_rec.has_value()) return *right_rec;
	// 右边撞到了的话 t 肯定更小
	if (left_rec.has_value()) return *left_rec;
	return std::nullopt;
}

// 划分

inline bool box_compare(const shared_ptr<hittable> a,
						const shared_ptr<hittable> b, int axis) {
	// 按照某个轴排序
	auto&& boxa = a->bounding_box(0, 0);
	auto&& boxb = b->bounding_box(0, 0);
	if (!boxa.has_value() || !boxb.has_value()) [[unlikely]] {
		throw std::invalid_argument(
			"hittable object don't have a bounding box");
	}
	return boxa->mn[axis] < boxb->mn[axis];
}

bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objs,
				   int beg, int ed, f8 tm0, f8 tm1) {
	// 包括 beg，不包括 ed
	auto objs = src_objs;	   // modifiable
	int axis = rand_i4(0, 2);  // [0, 2]
	auto comparator =
		bind(box_compare, std::placeholders::_1, std::placeholders::_2, axis);
	int obj_cnt = ed - beg;

	if (obj_cnt == 1) [[unlikely]] {
		ch[0] = ch[1] = objs[beg];
	} else if (obj_cnt == 2) {
		// 避免再递归一次，加速
		if (!comparator(objs[beg], objs[beg + 1])) {
			std::swap(objs[beg], objs[beg + 1]);
		}
		ch[0] = objs[beg];
		ch[1] = objs[beg + 1];
	} else {
		// 排序不包括最后一个
		sort(objs.begin() + beg, objs.begin() + ed, comparator);
		int mid = beg + obj_cnt / 2;
		ch[0] = std::make_shared<bvh_node>(objs, beg, mid, tm0, tm1);
		ch[1] = std::make_shared<bvh_node>(objs, mid, ed, tm0, tm1);
	}

	// 当前节点的撞击盒是两个子节点的和

	auto&& boxleft = ch[0]->bounding_box(tm0, tm1);
	auto&& boxright = ch[1]->bounding_box(tm0, tm1);

	if (!boxleft.has_value() || !boxright.has_value()) [[unlikely]] {
		throw std::runtime_error("hittable object don't have bouding box");
	}

	box = *boxleft + *boxright;
}