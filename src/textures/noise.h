#pragma once

#include <algorithm>
#include <array>
#include <functional>

#include "../perlin.h"
#include "../rtow.h"
#include "../texture.h"

template <int SZ>
class noise_texture : public texture {
   public:
    noise_texture(f8 _square_siz = 0.25) : pnoise(perlin<SZ>(_square_siz)) {}
    virtual color value(f8 polar, f8 azim, const pt3& p) const override {
        color&& ret = color(1, 1, 1) * pnoise.noise(p);
        return ret;
    }
    perlin<SZ> pnoise;  // perlin noise
};