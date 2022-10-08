#pragma once
#include "../noises/noises.h"
#include "../rtow.h"
#include "../texture.h"

class marblelike : public texture {
   public:
	color value(f8 polar, f8 azim, const pt3& p) const override {
        return color(1, 1, 1) *.5 * (1 + sin(p.z() + 10 * terb->noise_coeff(p)));
    }
	marblelike(shared_ptr<terbulence> _terb = make_shared<terbulence>()) : terb(_terb) {}
	shared_ptr<terbulence> terb;
};