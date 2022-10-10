#pragma once
#include <memory>
#include <utility>

#include "../rtow.h"
#include "./texture.h"
#include "fixed.h"

class checker_texture : public texture {
   public:
    checker_texture() = default;
    checker_texture(std::shared_ptr<texture> _odd,
                    std::shared_ptr<texture> _eve,
                    std::tuple<f8, f8, f8> _checker_coef = {10, 10, 10})
        : od(_odd), ev(_eve), checker_coef(_checker_coef) {}

    checker_texture(const color& c1, const color& c2,
                    std::tuple<f8, f8, f8> _checker_coef = {10, 10, 10})
        : od(make_shared<fixed_color>(c1)),
          ev(make_shared<fixed_color>(c2)),
          checker_coef(_checker_coef) {}

    virtual color value(f8 polar, f8 azim, const pt3& p) const override {
        // 如果是球体
        f8 sines = sin(p.x() * std::get<0>(checker_coef)) *
                   sin(p.y() * std::get<1>(checker_coef)) * 
                   sin(p.z() * std::get<2>(checker_coef));
        color&& ret =
            sines > 0 ? od->value(polar, azim, p) : ev->value(polar, azim, p);
        return ret;
    }

    std::shared_ptr<texture> od, ev;
    std::tuple<f8, f8, f8> checker_coef;
};