#pragma once
#include <optional>

#include "aabb.h"
#include "ray.h"
#include "rtow.h"
#include "vec3.h"

using namespace std;

class material;

struct hit_rec {
    vec3 hit_pt;  // 击中的点
    vec3 norm;    // 击中后反射的朝向
    f8 t;         // 解出来的 t
    f8 hori, verti;  // 球面上的坐标，使用极角和方位角算出来
    bool front_face; // 从物体的内部射入还是外部
    shared_ptr<material> mat_ptr;

    inline void set_face_normal(const ray& r, const vec3& outward_norm) {
        front_face =
            dot(r.dir, outward_norm) < 0;  // 小于 0 说明是从外部击中物体的
        // 点乘的定义 -> 3b1b
        // 为 1 的话是相同方向，反之亦然
        // 法线永远朝外
        norm = front_face ? outward_norm : -outward_norm;
    }
};

class hittable {
   public:
    virtual optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const = 0;
    virtual optional<aabb> bounding_box(f8 tm0, f8 tm1) const = 0;
    // 一个时间段内的撞击盒
};
