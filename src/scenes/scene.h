#pragma once
#include <vector>

#include "../backgrounds/backgrounds.h"
#include "../camera.h"
#include "../hittables/hittable.h"
#include "../rtow.h"


using color_map = std::vector<std::vector<color>>;

struct scene {
	scene(const shared_ptr<hittable> _world, const shared_ptr<background> _back, const shared_ptr<camera> _cam)
		: world(_world), back(_back), cam(_cam) {}


    // 不能用常量引用，常量引用引用的是一个临时变量，随后会被销毁
    shared_ptr<hittable> world;
	shared_ptr<background> back;
	shared_ptr<camera> cam;
};