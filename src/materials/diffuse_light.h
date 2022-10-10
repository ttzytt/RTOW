#pragma once
#include <optional>

#include "material.h"
#include "../rtow.h"
#include "../textures/texture.h"

class diffuse_light : public material {
	// 给纹理类换个皮
   public:
	diffuse_light(shared_ptr<texture> distrib) : emit_distribution(distrib){};
	diffuse_light(const color& c)
		: emit_distribution(make_shared<fixed_color>(c)){};

	std::optional<std::pair<ray, color>> ray_reflected(
		const ray& r_in, const hit_rec& rec) const override {
		return std::nullopt;
	}

	color emitted(f8 polar, f8 azim, const pt3& p) const override {
		return emit_distribution->value(polar, azim, p);
	}

	shared_ptr<texture> emit_distribution;	// 朝着不同方向发出的光不相同
};