#include <atomic>
#include <mutex>
#include <stack>
#include <thread>

#include "backgrounds/backgrounds.h"
#include "camera.h"
#include "externals/tqdm.h"
#include "hittables/hittable_list.h"
#include "materials/material.h"
#include "rtow.h"
#include "scenes/scene.h"

class renderer {
   public:
	struct block2rend {
		std::pair<int, int> col_range;
		std::pair<int, int> row_range;	// [)
	};

	struct config {
		std::pair<int, int> blksiz, picsiz;
		const int sample_per_pix = 500, max_dep = 130,
				  th_cnt = std::thread::hardware_concurrency();
	};

	using color_map = std::vector<std::vector<color>>;
	using job_stack = std::stack<block2rend>;

	renderer(const config& _conf) : conf(_conf) {}

	color ray_color(const ray& r, const hittable& world, const background& back,
					int dep_left) {
		if (dep_left <= 0) return color();

		auto&& rec = world.hit(r, 0.00001, infinity);
		if (!rec.has_value()) return back.value(r);	 // 如果没有撞到任何东西

		auto&& reflect_ret = rec->mat_ptr->ray_reflected(r, *rec);

		color emitted =
			rec->mat_ptr->emitted(rec->polar, rec->azim, rec->hit_pt);
		if (!reflect_ret.has_value())  // 如果物体不反射
			return emitted;

		auto& [r_ref, attenua] = *reflect_ret;
		return emitted + attenua * ray_color(r_ref, world, back, dep_left - 1);
	}

	void split_rend_work() {
		// 返回待渲染的块
		std::pair<int, int>&blksiz = conf.blksiz, picsiz = conf.picsiz;
		int lst_x = 0, lst_y = 0;
		std::vector<block2rend> tmp;
		for (int j = std::min(blksiz.second, picsiz.second);
			 j <= picsiz.second && lst_y != picsiz.second;
			 j += std::min(blksiz.second, picsiz.second - j)) {
			for (int i = std::min(blksiz.first, picsiz.first);
				 i <= picsiz.first && lst_x != picsiz.first;
				 i += std::min(blksiz.first, picsiz.first - i)) {
				tmp.push_back({{lst_x, i}, {lst_y, j}});
				lst_x = i;
			}
			lst_y = j;
			lst_x = 0;
		}
		static auto generator = std::mt19937();
		std::shuffle(tmp.begin(), tmp.end(), generator);
		// 渲染速度更均匀
		for(auto cur : tmp){
			jobs.push(cur);
		}
		tot_blk = jobs.size();
	}

	void rend_by_job(color_map& mp, const scene& sc) {
		while (true) {
			job_lock.lock();
			if (jobs.empty()) {
				job_lock.unlock();
				return;
			}
			block2rend cur_blk = jobs.top();
			jobs.pop();
			job_lock.unlock();
			auto [wid, hei] = conf.picsiz;
			for (int i = cur_blk.col_range.first; i < cur_blk.col_range.second;
				 i++) {
				for (int j = cur_blk.row_range.first;
					 j < cur_blk.row_range.second; j++) {
					color pixel(0, 0, 0);
					for (int k = 0; k < conf.sample_per_pix; k++) {
						f8 x = (i + rand_f8()) / (wid - 1);
						f8 y = (j + rand_f8()) / (hei - 1);
						ray r = sc.cam->get_ray(x, y);
						pixel +=
							ray_color(r, *sc.world, *sc.back, conf.max_dep);
					}
					mp[j][i] = pixel / (f8)conf.sample_per_pix;
				}
			}
			blk_completed++;
		}
	}

	color_map out_color_map(const scene& sc) {
		color_map mp;
		auto [wid, hei] = conf.picsiz;
		mp.resize(hei);
		for (int i = 0; i < hei; i++) mp[i].resize(wid);

		split_rend_work();
		std::thread bar_checker([&]() {	 // 检查进度
			while (blk_completed < tot_blk) {
				pbar.progress(blk_completed, tot_blk);
				usleep(100 * 1000);	 // 100 毫秒
			}
		});

		std::thread* ths[conf.th_cnt];
		for (int th = 0; th < conf.th_cnt; th++) {
			ths[th] = new std::thread(&renderer::rend_by_job, this,
									  std::ref(mp), std::ref(sc));
		}

		for (int i = 0; i < conf.th_cnt; i++) {
			ths[i]->join();
			delete ths[i];
		}
		bar_checker.join();
		pbar.finish();
		return mp;
	}
	void out_ppm(std::ostream& out, const color_map& mp) {
		out << "P3\n" << mp[0].size() << ' ' << mp.size() << "\n255\n";
		// 长宽
		for (int i = mp.size() - 1; i >= 0; i--) {
			auto& line = mp[i];
			for (auto& pix : line) {
				write_color(out, pix);
			}
		}
	}

	void out_ppm(std::ostream& out, const scene& sc) {
		out_ppm(out, out_color_map(sc));
	}

	config conf;

   private:
	std::mutex job_lock;
	job_stack jobs;
	tqdm pbar;
	int tot_blk;
	std::atomic<int> blk_completed;
};