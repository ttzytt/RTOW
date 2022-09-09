#pragma once
#include <functional>

#include "../hittable.h"
#include "../material.h"
#include "../rtow.h"
#include "sphere.h"

class moving_sphere;
using namespace std;

auto linear_mov = [](f8 x) { return x; };
// 输入 0 ~ 1，代表当前时间是运动总时间的百分之多少
auto square_mov = [](f8 x) { return x * x;};

class moving_sphere : public hittable {
   public:
    moving_sphere() {}

    moving_sphere(const pt3& _cent_beg, const pt3& _cent_ed, f8 _tm_beg,
                  f8 _tm_ed, f8 rad, shared_ptr<material> mat, function<f8(f8)> _mov_func = linear_mov)
        : cent_beg(_cent_beg),
          cent_ed(_cent_ed),
          tm_beg(_tm_beg),
          tm_ed(_tm_ed),
          radius(rad),
          mat_ptr(mat), 
          mov_func(_mov_func)
          {};

    optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override {
        return sphere(cent_by_tm(r.tm), radius, mat_ptr).hit(r, t_min, t_max);
    }

    pt3 cent_by_tm(f8 tm) const {
        f8 tm_percent = ((tm - tm_beg) / (tm_ed - tm_beg));
        vec3&& mov_vec = cent_ed - cent_beg;
        return cent_beg + mov_vec * mov_func(tm_percent);
    }

    function<f8(f8)> mov_func;  // 默认线性
    pt3 cent_beg, cent_ed;      // 开始和结束的圆心
    f8 tm_beg, tm_ed;           // 移动的时间
    f8 radius;
    shared_ptr<material> mat_ptr;
};