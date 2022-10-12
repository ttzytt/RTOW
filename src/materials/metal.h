#include <utility>

#include "../rtow.h"
#include "../textures/fixed.h"
#include "material.h"
#pragma once

class metal : public material {
   public:
	metal(const color& alb, f8 fuz = 0)
		: albedo(make_shared<fixed_color>(alb)), fuzz(fuz) {}
    metal(shared_ptr<texture> alb, f8 fuz = 0) : 
        albedo(alb), fuzz(fuz) {}

	virtual std::optional<std::pair<ray, color>> ray_reflected(
		const ray& r_in, const hit_rec& rec) const override {
		vec3&& reflec_dir =
			r_in.dir.unit_vec().reflect(rec.norm) + rand_unit_sphere() * fuzz;
		ray r_out(rec.hit_pt, reflec_dir, r_in.tm);

        color alb_ret = albedo->value(rec.polar, rec.azim, rec.hit_pt);
		if (dot(reflec_dir, rec.norm) > 0) return std::make_pair(r_out, alb_ret);
		return std::nullopt;
	}

	shared_ptr<texture> albedo;	 // 反射率
	f8 fuzz;					 // 模糊
};