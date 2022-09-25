#pragma once
#include "../rtow.h"

vec3 linear_map(const vec3 &x) { return x; };

vec3 _3t2_2t3(const vec3 &x) { return x * x * (vec3(3.0) - 2 * x); };
