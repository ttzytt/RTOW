#pragma once
#include <algorithm>
#include <array>
#include <functional>

#include "../noise.h"
#include "../rtow.h"
#include "./interpo_mapers.h"

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
class value_noise : public noise {
   public:
    value_noise(f8 _square_sz = 0.25, std::function<vec3(const vec3&)> _maper = _3t2_2t3)
        : square_sz(_square_sz), interpo_maper(_maper) {
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
        return rand_f8s[x_perm[i] ^ y_perm[j] ^ z_perm[k]];  // 不进位加法
    }

    inline f8 noise_coeff(const pt3& pt) const override {
        //根据 ptfl 来估计 pt 的值？
        vec3&& ptcoeff = pt * (1 / square_sz);
        vec3&& ptfl = ptcoeff.floor();
        vec3&& ts = ptcoeff - ptfl;           // 算出和 pt.floor 的偏移量
        vec3&& maped_ts = interpo_maper(ts);
        f8 nearby[2][2][2];

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    vec3&& cur = ptfl + vec3(i, j, k);
                    // ptlf 的基础上加上 1 的偏移量当作那 8 个点
                    nearby[i][j][k] = noise(cur);
                }
            }
        }
        return trilinear_interp(nearby, maped_ts);
    }

    f8 trilinear_interp(f8 c[2][2][2], const vec3& ts) const {
        // 书中的实现会快 6% 左右
        // 这段代码加了向量化运算速度还是差不多，可能是开了 O3
        // 之后已经用了指令集
        vec3 &&maped_ts = interpo_maper(ts);
        f8 accum = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) {
                    vec3 ijk(i, j, k);
                    vec3&& tmp = ijk * maped_ts + (1 - ijk) * (1 - maped_ts);
                    accum += tmp[0] * tmp[1] * tmp[2] * c[i][j][k];
                }
        return accum;
    }

    std::array<int, SIZ> x_perm, y_perm, z_perm;
    inline static std::array<f8, SIZ>&& rand_f8s =
        rand_f8s_initializer<SIZ>();  // 如果是静态变量会多次初始化？
    const f8 square_sz;
    const std::function<vec3(const vec3&)> interpo_maper;
};
