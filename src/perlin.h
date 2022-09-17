#pragma once
#include <algorithm>
#include <array>
#include <functional>

#include "rtow.h"

template <int SIZ>
std::array<f8, SIZ> rand_f8s_initializer() {
    std::array<f8, SIZ> ret;
    for (f8& num : ret) {
        num = rand_f8();
    }
    return ret;
}

template <int SIZ> 
// SIZ 开越大，灰度更多
// 但是 SIZ 必须是 2^x，要不然 & (SIZ - 1) 时就会损失很多精度 
class perlin {
   public:
    perlin(f8 _square_sz = 0.25) : square_sz(_square_sz) {
        static auto generator = std::mt19937();
        std::iota(x_perm.begin(), x_perm.end(), 0);
        std::shuffle(x_perm.begin(), x_perm.end(), generator);
        y_perm = x_perm;
        std::shuffle(y_perm.begin(), y_perm.end(), generator);
        z_perm = y_perm;
        std::shuffle(z_perm.begin(), z_perm.end(), generator);
    }
    
    inline f8 noise(const pt3& p) const {
        const f8 coeff = 1 / square_sz;
        int i = int(coeff * (p.x())) & (SIZ - 1);
        // coeff 越大，就能越快的经过 int 转换后变成另一个数字
        int j = int(coeff * (p.y())) & (SIZ - 1);
        int k = int(coeff * (p.z())) & (SIZ - 1);
        return rand_f8s[x_perm[i] ^ y_perm[j] ^ z_perm[k]]; // 不进位加法
    }



    std::array<int, SIZ> x_perm, y_perm, z_perm;
    inline static std::array<f8, SIZ>&& rand_f8s = rand_f8s_initializer<SIZ>(); // 如果是静态变量会多次初始化？
    const f8 square_sz;
};
