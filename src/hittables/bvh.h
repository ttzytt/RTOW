#pragma once
#include "../rtow.h"
#include "../hittable.h"
#include "../aabb.h"

// bouding box hierarchy

class bvh_node : public hittable {
    public: 
        bvh_node() = default;

    virtual optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;
    virtual optional<aabb> bounding_box(f8 tm0, f8 tm1) const override{
        return box;
    } // 一个时间段内的撞击盒

    shared_ptr<hittable> ch[2];
    aabb box;
}; 

optional<hit_rec> bvh_node::hit(const ray& r, f8 t_min, f8 t_max) const {
    if (!box.hit(r, t_min, t_max)) return nullopt;

    auto&& left_rec = ch[0]->hit(r, t_min, t_max);
    auto&& right_rec = ch[1]->hit(r, t_min, t_max);

    if(left_rec.has_value()) return *left_rec;
    if(right_rec.has_value()) return *right_rec;
    return nullopt;
}   
