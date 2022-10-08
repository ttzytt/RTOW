#pragma once
#include <algorithm>
#include <array>
#include <functional>

#include "../noise.h"
#include "../rtow.h"
#include "./interpo_mapers.h"

template <int SIZ>
std::array<vec3, SIZ> rand_uvecs_initializer() {
    std::array<vec3, SIZ> ret;
    for (vec3& cur : ret) {
        cur = rand_unit_vec();
    }
    return ret;
}
template <int SIZ>
// SIZ 开越大，灰度更多
// 但是 SIZ 必须是 2^x，要不然 & (SIZ - 1) 时就会损失很多精度
class perlin_noise : public noise {
   public:
    perlin_noise(f8 _square_sz = 1, std::function<vec3(const vec3&)> _maper = _3t2_2t3)
        : square_sz(_square_sz), interpo_maper(_maper) {
        static auto generator = std::mt19937();
        std::iota(x_perm.begin(), x_perm.end(), 0);
        std::shuffle(x_perm.begin(), x_perm.end(), generator);
        y_perm = x_perm;
        std::shuffle(y_perm.begin(), y_perm.end(), generator);
        z_perm = y_perm;
        std::shuffle(z_perm.begin(), z_perm.end(), generator);
    }

    inline vec3 noise(const pt3& p) const {
        const f8 coeff = 1 / square_sz;
        int i = int(coeff * (p.x())) & (SIZ - 1);
        // coeff 越大，就能越快的经过 int 转换后变成另一个数字
        int j = int(coeff * (p.y())) & (SIZ - 1);
        int k = int(coeff * (p.z())) & (SIZ - 1);
        return rand_uvecs[x_perm[i] ^ y_perm[j] ^ z_perm[k]];  // 不进位加法
    }

    inline f8 noise_coeff(const pt3& pt) const override {
        //根据 ptfl 来估计 pt 的值？
        vec3&& ptcoeff = pt * (1 / square_sz);
        vec3&& ptfl = ptcoeff.floor();
        vec3&& ts = ptcoeff - ptfl;  // 算出和 pt.floor 的偏移量
        vec3 nearby[2][2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    vec3&& cur = ptfl + vec3(i, j, k);
                    nearby[i][j][k] = noise(cur);
                }
            }
        }
        f8 interped = trilinear_interp(nearby, ts);
        // return interped;
        return (interped + 1.0) * 0.5;
    }

    f8 trilinear_interp(const vec3 c[2][2][2], const vec3& ts) const {
        // vec3 &&mapped_ts = ts * ts* (vec3(3.0) - 2 * ts);
        vec3 &&maped_ts = interpo_maper(ts);
        f8 accum = 0.0;
        
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) {
                    vec3 ijk(i, j, k);
                    vec3&& dis_vec = ts - ijk;  // 插值点和提供点的距离
                    vec3&& tmp = (ijk * maped_ts) + ((1 - ijk) * (1 - maped_ts));
                    accum +=
                        tmp[0] * tmp[1] * tmp[2] * dot(c[i][j][k], dis_vec);
                    // 因为和距离向量做了点乘，所以离得近的点得到的值也近（因为离 floor 点的距离相似）
                    // 同时使用随机单位向量避免了值噪声的块状噪声
                }
        return accum;
    }

    std::array<int, SIZ> x_perm, y_perm, z_perm;
    inline static std::array<vec3, SIZ>&& rand_uvecs =
        rand_uvecs_initializer<SIZ>();  // 如果是静态变量会多次初始化？
    const f8 square_sz;
    std::function<vec3(const vec3&)> interpo_maper;
};
