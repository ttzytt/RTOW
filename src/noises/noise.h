#pragma once
#include "../rtow.h"
class noise {
   public:
    virtual f8 noise_coeff(const pt3 &pt) const = 0;
};