#pragma once
#include <vector>

#include "../backgrounds/backgrounds.h"
#include "../camera.h"
#include "../hittable.h"
#include "../rtow.h"


using color_map = std::vector<std::vector<color>>;

class scene;

namespace render {
struct config;
color_map out_color_map(const scene sc, const config& conf);
void out_ppm(const color_map& mp, std::ostream& out);
}  // namespace render

class scene {
   public:
	color_map out_color_map(const render::config& conf) const {
		return render::out_color_map(*this, conf);
	}
	void out_ppm(std::ostream& out, const render::config& conf) const {
		render::out_ppm(out_color_map(conf), out);
	}

	scene(const shared_ptr<hittable> _world, const shared_ptr<background> _back, const shared_ptr<camera> _cam)
		: world(_world), back(_back), cam(_cam) {}


    // 不能用常量引用，常量引用引用的是一个临时变量，随后会被销毁
    shared_ptr<hittable> world;
	shared_ptr<background> back;
	shared_ptr<camera> cam;
};