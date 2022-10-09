#pragma once
#include "../rtow.h"

class background {
   public:
	virtual color value(const ray& r) const = 0;
};

class gradient : public background {
   public:
	gradient(const color& sta, const color& ed, int axis) {
		if (axis == 0) {
			x_sta = sta, x_ed = ed;
            y_sta = y_ed = color(1);
        }
		else {
			y_sta = sta, y_ed = ed;
            x_sta = x_ed = color(1);
        }
	}
	gradient(const color& _x_sta, const color& _x_ed, const color& _y_sta,
			 const color& _y_ed)
		: x_sta(_x_sta), x_ed(_x_ed), y_sta(_y_sta), y_ed(_y_ed) {}

	color value(const ray& r) const override {
		vec3 unit_dir(r.dir.unit_vec());
		f8 y = 0.5 * (unit_dir.y() + 1.0);
		f8 x = 0.5 * (unit_dir.x() + 1.0);
		color&& x_col = (1.0 - x) * x_sta + x * x_ed;
		color&& y_col = (1.0 - y) * y_sta + y * y_ed;
        return y_col;
		return x_col * y_col;
	}
	color x_sta, x_ed, y_sta, y_ed;
};

class pure : public background {
   public:
	constexpr pure(const color& _c) : c(_c) {}

	constexpr color value(const ray& r) const override { return c; }
	color c;
};

const gradient blue_sky_back(color(1), color(.5, .8, 1), 1);
const pure pure_black_back(color(0));

const auto blue_sky_back_ptr = make_shared<gradient>(color(1), color(.5, .8, 1), 1);
const auto pure_black_back_ptr = make_shared<pure>(color(0));