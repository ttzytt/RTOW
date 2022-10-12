#pragma once
#include <optional>
#include "../rtow.h"
#include "hittable.h"
#include "../aabb.h"

class translate : public hittable {
   public:
	translate(shared_ptr<hittable> orig, const vec3& _off)
		: ptr(orig), offset(_off) {}

	std::optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;
	std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override;

	shared_ptr<hittable> ptr;
	vec3 offset;
};

std::optional<hit_rec> translate::hit(const ray& r, f8 t_min,
									  f8 t_max) const {
	ray move_r(r.orig - offset, r.dir, r.tm);
	auto rec = ptr->hit(move_r, t_min, t_max);
	rec->hit_pt += offset;
	rec->set_face_normal(move_r, rec->norm);
	return rec;
}

std::optional<aabb> translate::bounding_box(f8 tm0, f8 tm1) const {
	auto box = ptr->bounding_box(tm0, tm1);
	if(!box.has_value()) return std::nullopt;
	return aabb(box->mn + offset, box->mx + offset);
}