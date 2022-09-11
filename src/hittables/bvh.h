#pragma once
#include "../aabb.h"
#include "../rtow.h"
#include "functional"
#include "hittables.h"

  std;

// bouding box hierarchy

class bvh_node : public hittable {
   public:
    bvh_node() = default;
    bvh_node(const hittable_list& list, f8 tm0, f8 tm1)
        : bvh_node(list.objs, 0, list.objs.size(), tm0, tm1) {}
    bvh_node(vector<shared_ptr<hittable>> objs, int beg, int ed, f8 tm0,
             f8 tm1);

    virtual optional<hit_rec> hit(const ray& r, f8 t_min,
                                  f8 t_max) const override;
    virtual optional<aabb> bounding_box(f8 tm0, f8 tm1) const override {
        return box;
    }  // 一个时间段内的撞击盒

    shared_ptr<hittable> ch[2];
    aabb box;
};

optional<hit_rec> bvh_node::hit(const ray& r, f8 t_min, f8 t_max) const {
    if (!box.hit(r, t_min, t_max)) return nullopt;

    auto&& left_rec = ch[0]->hit(r, t_min, t_max);
    auto&& right_rec = ch[1]->hit(r, t_min, t_max);

    if (left_rec.has_value()) return *left_rec;
    if (right_rec.has_value()) return *right_rec;
    return nullopt;
}

// 划分

inline bool box_compare(const shared_ptr<hittable> a,
                        const shared_ptr<hittable> b, int axis) {
    // 按照某个轴排序
    auto&& boxa = a->bounding_box(0, 0);
    auto&& boxb = b->bounding_box(0, 0);
    if (!boxa.has_value() || !boxb.has_value()) [[unlikely]] {
        throw invalid_argument("hittable object don't have a bounding box");
    }
    return boxa->mn[axis] < boxb->mn[axis];
}

bvh_node::bvh_node(vector<shared_ptr<hittable>> objs, int beg, int ed, f8 tm0,
                   f8 tm1) {
    // 包括 beg，不包括 ed
    int axis = rand_i4(0, 2);  // [0, 2]
      placeholders;
    auto comparator = bind(box_compare, _1, _2, axis);
    int obj_cnt = ed - beg;

    if (obj_cnt == 1) [[unlikely]] {
        ch[0] = ch[1] = objs.front();
    } /*  else if (obj_cnt == 2){

     } */
    else {
        sort(objs.begin(), objs.begin() + ed - 1, comparator);
        int mid = beg + obj_cnt / 2;
        ch[0] = make_shared<bvh_node>(objs, beg, mid, tm0, tm1);
        ch[1] = make_shared<bvh_node>(objs, mid, ed, tm0, tm1);
    }

    // 当前节点的撞击盒是两个子节点的和

    auto&& boxleft = ch[0]->bounding_box(tm0, tm1);
    auto&& boxright = ch[1]->bounding_box(tm0, tm1);
    
    if(!boxleft.has_value() || !boxright.has_value()) [[unlikely]] {
        throw runtime_error("hittable object don't have bouding box");
    }

    box = *boxleft + *boxright;
}