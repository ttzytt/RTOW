#include <memory>
#include <vector>
#include "../hittable.h"
#include "../rtow.h"
#pragma once
using namespace std;

class hittable_list : public hittable {
   public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> obj) { add(obj); }

    void clear() { objs.clear(); }
    void add(shared_ptr<hittable> obj) { objs.push_back(obj); }

    virtual optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;

    vector<shared_ptr<hittable>> objs;
};

optional<hit_rec> hittable_list::hit(const ray& r, f8 t_min, f8 t_max) const {
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
    return nullopt;
}