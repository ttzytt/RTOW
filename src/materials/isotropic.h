#pragma once
#include "../rtow.h"
#include "../textures/fixed.h"
#include "material.h"

class isotropic : public material {
   public:
	isotropic(const color& c) : albedo(make_shared<fixed_color>(c)) {}
	isotropic(shared_ptr<texture> alb) : albedo(alb) {}

	std::optional<std::pair<ray, color>> ray_reflected(
		const ray& r_in, const hit_rec& rec) const override {
            color alb_ret = albedo->value(rec.polar, rec.azim, rec.hit_pt);
            ray r_ret(rec.hit_pt, rand_unit_vec(), r_in.tm);
            return std::make_pair(r_ret, alb_ret);
        }

	shared_ptr<texture> albedo;
};