#include <cmath>
#include <limits>
#include <memory>


#pragma once

// settings
const int MX_COLOR = 255;


// usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

using f16 = long double;// 16 字节浮点
using f8 = double;      // 8 字节浮点
using f4 = float;       // 4 字节浮点
using deg = f8;
using rad = f8;

using i1 = char;        // 1 字节整数
using i2 = short;       // 2 字节整数
using i4 = int;         // 4 字节整数
using i8 = long long;   // 8 字节整数
using i16 = __int128_t; // 16 字节整数


// constants
const f8 infinity = std::numeric_limits<f8>::infinity();
const f8 pi = 3.1415926535897932385;

// utils
inline f8 deg2rad(f8 deg){
    return deg * pi / 180.0;
}

inline f8 clamp(f8 x, f8 mn, f8 mx){
    if(x < mn) return mn;
    if(x > mx) return mx;
    return x;
}

inline f8 base_log(f8 x, f8 base) { return log(x) / log(base); }

#include "vec3.h"

#define AVX2
#ifdef AVX2
using namespace avx2;
#else
using namespace navx2;
#endif

#include "./rand_utils.h"

#include "ray.h"
#include "color.h"