#include "material.h"
#include "../textures/textures.h"
#include "../rtow.h"
#include <memory>
#include <utility>
#pragma once

class lambertian : public material {
   public:
    
    lambertian(const color& alb) : albedo(std::make_shared<fixed_color>(alb)) {}
    lambertian(std::shared_ptr<texture> text) : albedo(text) {}

    virtual std::optional<std::pair<ray, color>> 
    ray_reflected(const ray& r_in, const hit_rec& rec) const override {
        vec3 ref_dir = rec.norm + rand_unit_vec();

        if(ref_dir.near_zero()) // 如果 rand_unit_vec() 等于 -rec.norm 
            ref_dir = rec.norm;

        ray ref_ray(rec.hit_pt, ref_dir, r_in.tm);
        
        color &&attenue = albedo->value(rec.polar, rec.azim, rec.hit_pt);

        return std::make_pair(ref_ray, attenue);    
    }
    std::shared_ptr<texture> albedo;  // 材质不同位置的反射率不同
};