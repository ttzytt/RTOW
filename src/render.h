#pragma once
#include <array>
#include <atomic>
#include <list>
#include <optional>
#include <thread>

#include "backgrounds/backgrounds.h"
#include "camera.h"
#include "externals/tqdm.h"
#include "hittables/hittable_list.h"
#include "materials/material.h"
#include "rtow.h"
#include "scenes/scene.h"

using color_map = std::vector<std::vector<color>>;

// class render {

namespace render {
struct block2rend {
	std::pair<int, int> row_range;
	std::pair<int, int> col_range;	// inclusive
};

struct config {
	const int wid = 300, hei = 200, sample_per_pix = 500, max_dep = 130,
			  th_cnt = std::thread::hardware_concurrency();
};

color ray_color(const ray& r, const hittable& world, const background& back,
				int dep_left) {
	if (dep_left <= 0) return color();

	auto&& rec = world.hit(r, 0.00001, infinity);
	if (!rec.has_value()) return back.value(r);	 // 如果没有撞到任何东西

	auto&& reflect_ret = rec->mat_ptr->ray_reflected(r, *rec);

	color emitted = rec->mat_ptr->emitted(rec->polar, rec->azim, rec->hit_pt);
	if (!reflect_ret.has_value())  // 如果物体不反射
		return emitted;

	auto& [r_ref, attenua] = *reflect_ret;
	return emitted + attenua * ray_color(r_ref, world, back, dep_left - 1);
}

color_map out_color_map(scene sc, const config& conf) {
	std::atomic<int> line_completed = 0;
	int per_th = (conf.hei + conf.th_cnt - 1) / conf.th_cnt;
	int hei_left = conf.hei;
	color_map mp;
	mp.resize(conf.hei);
	for (int i = 0; i < conf.hei; i++) {
		mp[i].resize(conf.wid);
	}
	std::thread* ths[conf.th_cnt];
	tqdm pbar;

	std::thread bar_checker([&]() {	 // 检查进度
		while (line_completed < conf.hei) {
			pbar.progress(line_completed, conf.hei);
			usleep(100 * 1000);	 // 100 毫秒
		}
	});

	for (int th = 1; th <= conf.th_cnt; th++) {
		ths[th] = new std::thread(
			[&](int sta_line, int th_id) {
				for (int j = sta_line - 1; j >= std::max(sta_line - per_th, 0);
					 j--) {
					for (int i = 0; i < conf.wid; i++) {
						color pixel(0, 0, 0);
						for (int k = 0; k < conf.sample_per_pix; k++) {
							f8 x = (i + rand_f8()) / (conf.wid - 1);
							f8 y = (j + rand_f8()) / (conf.hei - 1);
							ray r = sc.cam->get_ray(x, y);
							pixel +=
								ray_color(r, *sc.world, *sc.back, conf.max_dep);
						}
						mp[j][i] = pixel / (f8)conf.sample_per_pix;
					}
					line_completed++;
				}
			},
			hei_left, th);
		hei_left -= per_th;
	}

	for (int i = 1; i <= conf.th_cnt; i++) {
		ths[i]->join();
		delete ths[i];
	}
	bar_checker.join();
	pbar.finish();
	return mp;
}

void out_ppm(const color_map& mp, std::ostream& out) {
	out << "P3\n" << mp[0].size() << ' ' << mp.size() << "\n255\n";
	// 长宽
	for (int i = mp.size() - 1; i >= 0; i--) {
		auto& line = mp[i];
		for (auto& pix : line) {
			write_color(out, pix);
		}
	}
}

}  // namespace render