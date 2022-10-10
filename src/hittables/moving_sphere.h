#pragma once
#include <functional>

#include "hittable.h"
#include "../materials/material.h"
#include "../rtow.h"
#include "sphere.h"

class moving_sphere;

auto linear_mov = [](f8 x) { return x; };
// 输入 0 ~ 1，代表当前时间是运动总时间的百分之多少
auto square_mov = [](f8 x) { return x * x; };

class moving_sphere : public hittable {
   public:
    moving_sphere() = default;

    moving_sphere(const pt3& _cent_beg, const pt3& _cent_ed, f8 _tm_beg,
                  f8 _tm_ed, f8 rad, shared_ptr<material> mat,
                  std::function<f8(f8)> _mov_func = linear_mov)
        : cent_beg(_cent_beg),
          cent_ed(_cent_ed),
          tm_beg(_tm_beg),
          tm_ed(_tm_ed),
          radius(rad),
          mat_ptr(mat),
          mov_func(_mov_func){};

    std::optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override {
        return sphere(cent_by_tm(r.tm), radius, mat_ptr).hit(r, t_min, t_max);
    }

    virtual std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override;

    pt3 cent_by_tm(f8 tm) const {
        f8 tm_percent = ((tm - tm_beg) / (tm_ed - tm_beg));
        vec3&& mov_vec = cent_ed - cent_beg;
        return cent_beg + mov_vec * mov_func(tm_percent);
    }

    std::function<f8(f8)> mov_func;  // 默认线性
    pt3 cent_beg, cent_ed;      // 开始和结束的圆心
    f8 tm_beg, tm_ed;           // 移动的时间
    f8 radius;
    shared_ptr<material> mat_ptr;
};

std::optional<aabb> moving_sphere::bounding_box(f8 tm0, f8 tm1) const {
    aabb init_box(
        cent_by_tm(tm0) - vec3(radius),
        cent_by_tm(tm0) + vec3(radius)
    );

    aabb end_box(
        cent_by_tm(tm1) - vec3(radius), // 三个分量都是 rad
        cent_by_tm(tm1) + vec3(radius)
    );

    return init_box + end_box; // 返回一个能同时覆盖 initbox 和 endbox 的大的撞击盒
}