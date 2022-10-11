#include "../rtow.h"
#include "hittable.h"
#pragma once
#include <optional>

template <int AXIS>
class rotate : public hittable {
   public:
	rotate(shared_ptr<hittable> orig, deg angle);
	std::optional<hit_rec> hit(const ray& r, f8 t_min, f8 t_max) const override;
	std::optional<aabb> bounding_box(f8 tm0, f8 tm1) const override;
	shared_ptr<hittable> ptr;
	f8 sin_the, cos_the;
	aabb box;
};

template <int AXIS>
rotate<AXIS>::rotate(shared_ptr<hittable> orig, deg angle) {
	sin_the = std::sin(angle), cos_the = std::cos(angle);
	box = ptr->bounding_box(0, infinity);  // original box
    // 可能没有盒
	pt3 mn(infinity), mx(-infinity);

	for (int i = 0; i < 2; i++) {
		// 选择八个端点中的一个
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				pt3 cur{i ? box.mn.x() : box.mx.x(),
						j ? box.mn.y() : box.mx.y(),
						k ? box.mn.z() : box.mx.z()};
				cur = cur.rotate(sin_the, cos_the, AXIS);

				mn = vmin(mn, cur);
				mx = vmax(mx, cur);
			}
		}
	}
	box = aabb(mn, mx);
}

template <int AXIS>
std::optional<hit_rec> rotate<AXIS>::hit(const ray& r, f8 t_min,
										 f8 t_max) const {
	ray rotated{
        r.orig.rotate(-sin_the, cos_the), // 旋转 - theta
        r.dir.rotate(-sin_the, cos_the),
        r.tm
    };
    auto rec = ptr->hit(rotated, t_min, t_max);
    if(!rec.has_value()) return std::nullopt;

    rec->hit_pt = rec->hit_pt.rotate(sin_the, cos_the);
    rec->norm = rec->norm.rotate(sin_the, cos_the);
	rec->set_face_normal(rotated, rec->norm);
	return rec;
}