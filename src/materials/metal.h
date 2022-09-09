#include "../material.h"
#include "../rtow.h"
#pragma once
using namespace std;

class metal : public material {
   public:
    metal(const color& alb, f8 fuz = 0) : albedo(alb), fuzz(fuz) {}

    virtual optional<pair<ray, color>> get_ray_out(
        const ray& r_in, const hit_rec& rec) const override {
        vec3&& reflec_dir = r_in.dir.unit_vec().reflect(rec.norm) + rand_unit_sphere() * fuzz;
        ray r_out(rec.hit_pt, reflec_dir, r_in.tm);

        if(dot(reflec_dir, rec.norm) > 0)
            return make_pair(r_out, albedo);
        return nullopt;
    }

    vec3 albedo;  // 反射率
    f8 fuzz;      // 模糊
};