#include <cmath>
#include <limits>
#include <memory>
#include <functional>
#include <random>

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

inline f8 rand_f8(){
    static std::uniform_real_distribution<f8> distribution(0, 1);
    // 不是静态的话每次调用返回值都相同
    static std::mt19937 generator;
    // static std::function<f8()> rand_generator = std::bind(distribution, generator);
    // return rand_generator();
    return distribution(generator); // 分布接收一个随机发生器参数
}

inline f8 rand_f8(f8 mn, f8 mx){
    return mn + (mx - mn) * rand_f8();
}

inline f8 clamp(f8 x, f8 mn, f8 mx){
    if(x < mn) return mn;
    if(x > mx) return mx;
    return x;
}


#include "ray.h"
#include "vec3.h"
#include "color.h"