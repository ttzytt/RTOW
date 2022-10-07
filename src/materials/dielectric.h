#include "../material.h"
#include "../rtow.h"
#pragma once

class dielectric : public material {
   public:
    dielectric(f8 ref, const color& alb = color(1, 1, 1), f8 fuz = 0)
        : ref_idx(ref), albedo(alb), fuzz(fuz) {}

    virtual std::optional<std::pair<ray, color>> ray_reflected(
        const ray& r_in, const hit_rec& rec) const override {
        f8 ref_ratio = rec.front_face ? (1.0 / ref_idx) : ref_idx;
        // 光线从空气进入玻璃球还是玻璃球进入空气？

        // vec3&& refract_dir = r_in.dir.unit_vec().refract(rec.norm, ref_ratio)
        // +
        //                      rand_unit_sphere() * fuzz;
        vec3 unit_dir = r_in.dir.unit_vec();
        f8 cos_theta = fmin(dot(-unit_dir, rec.norm), 1.0);
        f8 sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = ref_ratio * sin_theta > 1.0;
        vec3 out_dir;
        if (cannot_refract)
            out_dir = reflect(unit_dir, rec.norm);
        else
            out_dir = refract(unit_dir, rec.norm, ref_ratio);
        out_dir += rand_unit_sphere() * fuzz;
        ray r_out(rec.hit_pt, out_dir, r_in.tm);
        return std::make_pair(r_out, albedo);
    }

    f8 ref_idx, fuzz;  // 折射率
    color albedo;      // 衰减
};