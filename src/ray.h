#pragma once
#include "rtow.h"

class ray {
   public:
    ray() = default;
    ray(const pt3& _orig, const vec3& _dir, const f8& _tm) : orig(_orig), dir(_dir), tm(_tm) {}

    pt3 at(double dis) const {
        return orig + dis * dir;
    }
    
    pt3 orig;
    vec3 dir;
    f8 tm;
};