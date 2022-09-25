#pragma once

#include <algorithm>
#include <array>
#include <functional>

#include "../noises/perlin.h"
#include "../rtow.h"
#include "../texture.h"
#include "../noise.h"

template <int SZ>
class noise_texture : public texture {
   public:
    noise_texture(shared_ptr<noise> _ns) : ns(_ns) {}
    virtual color value(f8 polar, f8 azim, const pt3& p) const override {
        color&& ret = color(1, 1, 1) * ns->noise_coeff(p);
        return ret;
    }

    shared_ptr<noise> ns;
};