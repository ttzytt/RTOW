#pragma once
#include <optional>
#include <utility>

#include "hittable.h"
#include "rtow.h"

class material {
   public:
    virtual std::optional<std::pair<ray, color>> ray_reflected(
        const ray& r_in, const hit_rec& rec) const = 0;
    // 返回光线和颜色衰减
    // 可能会没有光线，因为衰减过多，或者这个地方不返回光线
    virtual color emitted(f8 polar, f8 azim, const pt3& p) const {
        return color(0);
    }
};