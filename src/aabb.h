#pragma once
#include <cmath>

#include "rtow.h"

// axis-aligned bounding boxes
class aabb {
   public:
    aabb() = default;
    aabb(const pt3& _mn, const pt3& _mx) : mn(_mn), mx(_mx) {}
    // x0 = a_x + b_x * t
    // t = (x_0 - a_x) / (b_x)
    bool hit(const ray& r, f8 tmin, f8 tmax) const {
        for (int dim = 0; dim < 3; dim++) {
            f8 invD = 1.0 / r.dir[dim];  // 需要考虑正负

            f8 t0 = (mn[dim] - r.orig[dim]) * invD;
            // 应该先穿过 t0
            f8 t1 = (mx[dim] - r.orig[dim]) * invD;

            if (invD < 0.0) std::swap(t0, t1);  // 如果光线是反方向来的

            tmin = std::max(tmin, t0);  // x, y, z 轴中最大的 tmin
            tmax = std::min(tmax, t1);

            // 如果碰到 tmin 之前先碰到 tmax

            if (tmax <= tmin) return false;
        }
        return true;
    }

    void operator+=(aabb box);

    pt3 mn, mx;  // 两个点确定的立方体，应该先碰到 mn 延申出来的面
};

aabb surrounding_box(aabb b1, aabb b2) {
    pt3 mn(std::min(b1.mn[0], b2.mn[0]), std::min(b1.mn[1], b2.mn[1]),
           std::min(b1.mn[2], b2.mn[2]));

    pt3 mx(std::max(b1.mx[0], b2.mx[0]), std::max(b1.mx[1], b2.mx[1]),
           std::max(b1.mx[2], b2.mx[2]));

    return aabb(mn, mx);
}

inline aabb operator+(aabb b1, aabb b2) { return surrounding_box(b1, b2); }

void aabb::operator+=(aabb box) { *this = surrounding_box(*this, box); }