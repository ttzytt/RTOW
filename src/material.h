#pragma once
#include <optional>
#include <utility>

#include "hittable.h"
#include "rtow.h"
using namespace std;

class material {
   public:
    virtual optional<pair<ray, color>> get_ray_out(
        const ray& r_in, const hit_rec& rec) const = 0;
    // 返回光线和颜色衰减
    // 可能会没有光线，因为衰减过多，或者这个地方不返回光线
};