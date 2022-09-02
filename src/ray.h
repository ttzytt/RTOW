#pragma once
#include "vec3.h"

class ray {
   public:
    ray(){}
    ray(const pt3& _orig, const vec3& _dir) : orig(_orig), dir(_dir) {}

    pt3 at(double dis) const {
        return orig + dis * dir;
    }

    pt3 orig;
    vec3 dir;
};