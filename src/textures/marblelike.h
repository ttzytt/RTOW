#pragma once
#include "../noises/noises.h"
#include "../rtow.h"
#include "./texture.h"

class marblelike : public texture {
   public:

	color value(f8 polar, f8 azim, const pt3& p) const override {
		return color(1, 1, 1) * .5 *
			   (1 + sin(p.x() * line_freq + 10 * terb->noise_coeff(p)));
		// 这里的 p.x() 会让一条看起来暗一点一条看起来亮一点
	}
	marblelike(shared_ptr<terbulence> _terb = make_shared<terbulence>(),
			   f8 _line_freq = 1)
		: terb(_terb), line_freq(_line_freq) {}
	shared_ptr<terbulence> terb;
	f8 line_freq;
};