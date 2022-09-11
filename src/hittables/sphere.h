#include <optional>

#include "../hittable.h"
#include "../rtow.h"
#include "../vec3.h"
#pragma once


class sphere : public hittable {
   public:
    sphere() = default;
    sphere(const pt3 &cent, f8 rad, shared_ptr<material> mat)
        : center(cent), radius(rad), mat_ptr(mat){};

    virtual std::optional<hit_rec> hit(const ray& r, f8 t_min,
                                  f8 t_max) const override;

    virtual std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override;    

    // optional<hit_rec> hit2(const ray& r, f8 t_min, f8 t_max) const;
    pt3 center;
    f8 radius;
    shared_ptr<material> mat_ptr;
};

std::optional<hit_rec> sphere::hit(const ray& r, f8 t_min, f8 t_max) const  {
    vec3 oc = r.orig - center;  // 圆心和光线起点的差
    f8 a = dot(r.dir, r.dir);   // 系数
    f8 b = 2.0 * dot(oc, r.dir);
    f8 c = dot(oc, oc) - radius * radius;
    f8 discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt;
    }

    f8 t_root = (-b - sqrt(discriminant)) / (2 * a);
    if (t_root < t_min || t_max < t_root) {
        t_root = ((-b + sqrt(discriminant)) / (2 * a));
        if (t_root < t_min || t_max < t_root) return std::nullopt;
    }

    hit_rec rec;
    rec.t = t_root;
    rec.hit_pt = r.at(rec.t);
    vec3 outward_norm = (rec.hit_pt - center) / radius;  // 确保单位长度
    rec.set_face_normal(r, outward_norm);
    rec.mat_ptr = mat_ptr;

    return rec;
}

std::optional<aabb> sphere::bounding_box(f8 tm0, f8 tm1) const {
    return aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius)
    );
}

// optional<hit_rec> sphere::hit(const ray& r, f8 t_min, f8 t_max) const {
//     vec3 oc = r.orig - center;
//     auto a = r.dir.len_sq();
//     auto half_b = dot(oc, r.dir);
//     auto c = oc.len_sq() - radius * radius;

//     auto discriminant = half_b * half_b - a * c; // 实际上是 1/4 的 dis
//     if (discriminant < 0) return nullopt;
//     auto sqrtd = sqrt(discriminant);

//     // Find the nearest root that lies in the acceptable range.
//     auto root = (-half_b - sqrtd) / a;
//     if (root < t_min || t_max < root) {
//         root = (-half_b + sqrtd) / a;
//         if (root < t_min || t_max < root) return nullopt;
//     }

//     hit_rec rec;
//     rec.t = root;
//     rec.hit_pt = r.at(rec.t);
//     vec3 outward_normal = (rec.hit_pt - center) / radius;
//     rec.set_face_normal(r, outward_normal);
//     rec.mat_ptr = mat_ptr;
//     return rec;
// }