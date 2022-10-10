#include <memory>
#include <vector>
#include "hittable.h"
#include "../rtow.h"
#include "../aabb.h"
#include <optional>
#pragma once

class hittable_list : public hittable {
   public:
    hittable_list() = default;
    hittable_list(shared_ptr<hittable> obj) { add(obj); }

    void clear() { objs.clear(); }
    void add(shared_ptr<hittable> obj) { 
        objs.push_back(obj); 
    }

    virtual std::optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;
    virtual std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override;

    std::vector<shared_ptr<hittable>> objs;
};

std::optional<hit_rec> hittable_list::hit(const ray& r, f8 t_min, f8 t_max) const {
    hit_rec&& tmp_rec = hit_rec(); // 一定要初始化？？？
    bool hit_anything = false;
    f8 t_closest = t_max; // 目前为止最接近相机的 t

    for(auto& obj : objs){
        auto&& ret = obj->hit(r, t_min, t_closest);
        // 这样不复制临时对象
        if(ret.has_value()){
            hit_anything = true;
            t_closest = ret->t;
            tmp_rec = *ret;
        }
    }

    if(hit_anything)
        return tmp_rec;
    return std::nullopt;
}

std::optional<aabb> hittable_list::bounding_box(f8 tm0, f8 tm1) const {
    if(objs.empty()) return std::nullopt;
    bool first_box;
    aabb ret;
    for(auto& obj : objs){
        auto&& cur_box = obj->bounding_box(tm0, tm1);
        if(!cur_box.has_value())
            return std::nullopt;
        ret += *cur_box; // 每次都扩大一些
    }
    return ret;
}