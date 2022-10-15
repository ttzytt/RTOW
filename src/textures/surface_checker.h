#pragma once
#include <memory>
#include <utility>

#include "../rtow.h"
#include "./texture.h"
#include "fixed.h"

class surface_checker : public texture {
   public:
	using text_array = std::vector<std::shared_ptr<texture>>;
	surface_checker() = default;
	surface_checker(const text_array& _texts,
					const std::pair<f8, f8> _siz = {514, 114})
		: texts(_texts), polar_azim_siz(_siz) {}

	virtual color value(f8 polar, f8 azim, const pt3& p) const override {
		int x_idx = (i8)(azim * polar_azim_siz.first) % texts.size();
		int y_idx = (i8)(polar * polar_azim_siz.second / 2.0) % texts.size();
		return texts[(x_idx + y_idx) % texts.size()]->value(polar, azim, p);
	}

	text_array texts;
	std::pair<f8, f8> polar_azim_siz;  // 垂直方向和水平方向有多少格
};