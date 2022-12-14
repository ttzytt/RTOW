#pragma once
#include <optional>

#include "../materials/isotropic.h"
#include "../materials/material.h"
#include "../rtow.h"
#include "hittable.h"

// 距离为 1 的路径中有 n 个粒子，有 p 的概率穿过一个粒子，那么穿过距离为 d
// 的雾的概率就是 f(d) = p^(nd)
// 我们希望能按照上面的概率随机生成每束光线穿过雾的距离
// 那么可以得到 d = log_p(rand())*(1/n)，也上函数的反函数。其中 rand
// 产生的是一个概率

class const_fog : public hittable {
   public:
	const_fog(shared_ptr<hittable> bound, shared_ptr<material> _parti_mat,
			  f8 particle_p_unit = 0.01145, f8 _passing_prob = 0.368)
		: boundary(bound),
		  parti_mat(_parti_mat),
		  passing_prob(_passing_prob),
		  inv_dens(1.0 / particle_p_unit) {}
	const_fog(shared_ptr<hittable> bound, color c, 
			  f8 particle_p_unit = 0.01145, f8 _passing_prob = 0.368)
		: boundary(bound),
		  parti_mat(make_shared<isotropic>(c)),
		  passing_prob(_passing_prob),
		  inv_dens(1.0 / particle_p_unit) {}

	std::optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;
	std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override{
		return boundary->bounding_box(tm0, tm1);
	};
	shared_ptr<hittable> boundary;
	shared_ptr<material> parti_mat;

	f8 inv_dens;
	f8 passing_prob;
};

std::optional<hit_rec> const_fog::hit(const ray& r, f8 t_min, f8 t_max) const {
	auto&& rec_in = boundary->hit(r, -infinity, t_max);	 // 撞到烟雾外围的记录
	if (!rec_in.has_value()) return std::nullopt;
	auto&& rec_out = boundary->hit(r, rec_in->t + 0.0001, infinity);
	// 这里的 t_min 和 t_max 只代表进出边界
	// 比如就算 ray_out 的 t 大于 t_max，他还是有可能在 t_max 之前就从雾中出去了
	if (!rec_out.has_value()) return std::nullopt; // 必须穿入再穿出

	// 但是如果 ray_out 的 t 比 t_min 或者 rec_in 还小就有问题了，因为根本不可能进入边界
	// 也就是两个区间必须重合
	rec_in->t = std::max(rec_in->t, t_min);
	rec_out->t = std::min(rec_out->t, t_max);

	if (rec_in->t >= rec_out->t) return std::nullopt;

	f8 ray_len = r.dir.len();
	f8 dis_in_bound = (rec_out->t - rec_in->t) * ray_len;
	f8 hit_dis = inv_dens * base_log(rand_f8(), passing_prob);
	if (hit_dis > dis_in_bound) return std::nullopt;

	hit_rec ret = *rec_in;
	ret.t = rec_in->t + hit_dis / ray_len;
	if(ret.t > t_max || ret.t < t_min) return std::nullopt;
	ret.hit_pt = r.at(ret.t);

	ret.norm = rand_unit_vec();
	ret.front_face = true;	// 不会从雾粒子里出来
	ret.mat_ptr = parti_mat;
	return ret;
}