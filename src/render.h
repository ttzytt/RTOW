#pragma once
#include <array>
#include <atomic>
#include <optional>
#include <thread>

#include "camera.h"
#include "hittable.h"
#include "hittables/hittable_list.h"
#include "material.h"
#include "rtow.h"
#include "tqdm.h"

using namespace std;
using color_map = vector<vector<color>>;

// class render {
namespace render {
color ray_color(const ray& r, const hittable& world, int dep_left) {
    if (dep_left <= 0) return color(0, 0, 0);

    auto&& rec = world.hit(r, 0.00001, infinity);
    if (rec.has_value()) {
        auto&& reflect_ret = rec->mat_ptr->get_ray_out(r, *rec);
        if (reflect_ret.has_value()) {
            auto& [r_ref, attenua] = *reflect_ret;

            return attenua * ray_color(r_ref, world, dep_left - 1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_dir = unit_vec(r.dir);  // r.dir 是屏幕上的点
    f8 t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.8, 1.0);
    // 蓝白渐变的背景
}

color_map out_color_map(const camera& cam, const hittable_list& world,
                        int wid = 300, int hei = 200, int sample_per_pix = 500,
                        int max_dep = 130,
                        int th_cnt = thread::hardware_concurrency()) {
    atomic<int> line_completed = 0;
    int per_th = (hei + th_cnt - 1) / th_cnt;
    int hei_left = hei;
    color_map mp;
    mp.resize(hei);
    for (int i = 0; i < hei; i++) {
        mp[i].resize(wid);
    }
    thread* ths[th_cnt];
    tqdm pbar;

    thread bar_checker([&]() { // 检查进度
        while (line_completed < hei) {
            pbar.progress(line_completed, hei);
            usleep(100 * 1000);  // 100 毫秒
        }
    });
    
    for (int th = 1; th <= th_cnt; th++) {
        ths[th] = new thread(
            [&](int sta_line, int th_id) {
                for (int j = sta_line - 1; j >= max(sta_line - per_th, 0);
                     j--) {
                    for (int i = 0; i < wid; i++) {
                        color pixel(0, 0, 0);
                        for (int k = 0; k < sample_per_pix; k++) {
                            f8 x = (i + rand_f8()) / (wid - 1);
                            f8 y = (j + rand_f8()) / (hei - 1);
                            ray r = cam.get_ray(x, y);
                            pixel += ray_color(r, world, max_dep);
                        }
                        mp[j][i] = pixel / (f8)sample_per_pix;
                    }
                    line_completed++;
                }
            },
            hei_left, th);
        hei_left -= per_th;
    }

    for (int i = 1; i <= th_cnt; i++) {
        ths[i]->join();
        delete ths[i];
    }
    bar_checker.join();
    pbar.finish();
    return mp;
}

void out_ppm(const color_map& mp, ostream& out) {
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