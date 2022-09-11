#include "../material.h"
#include "../rtow.h"
#pragma once
  std;

class lambertian : public material {
   public:
    lambertian(const color& alb) : albedo(alb) {}
    
    virtual optional<pair<ray, color>> 
    get_ray_out(const ray& r_in, const hit_rec& rec) const override {
        vec3 ref_dir = rec.norm + rand_unit_vec();

        if(ref_dir.near_zero()) // 如果 rand_unit_vec() 等于 -rec.norm 
            ref_dir = rec.norm;

        ray ref_ray(rec.hit_pt, ref_dir, r_in.tm);
        return make_pair(ref_ray, albedo);    
    }
    color albedo;  // 反射率
};