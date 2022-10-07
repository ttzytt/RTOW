#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <random>
#include <vector>
inline f8 rand_f8() {
    static std::uniform_real_distribution<f8> distribution(0, 1); // [)
    // 不是静态的话每次调用返回值都相同
    static std::mt19937 generator;
    // static std::function<f8()> rand_generator = std::bind(distribution,
    // generator); return rand_generator();
    return distribution(generator);  // 分布接收一个随机发生器参数
}


inline f8 rand_f8(f8 mn, f8 mx) { return mn + (mx - mn) * rand_f8(); }

inline i8 rand_i8(i8 mn, f8 mx) { return rand_f8(mn, mx + 1); }

inline i4 rand_i4(i4 mn, i4 mx) { return rand_f8(mn, mx + 1); }

inline vec3 rand_unit_sphere() {
    // 圆心为 (0, 0)
    while (true) {
        vec3 pt = vec3::rand(-1, 1);
        if (pt.len_sq() >= 1) continue;
        return pt;
    }
}

inline vec3 rand_unit_vec() {
    f8 angle = rand_f8(0, 2 * pi);
    f8 z = rand_f8(-1, 1);

    f8 r = sqrt(1 - z * z);
    return vec3(r * cos(angle), r * sin(angle), z);
}

inline vec3 rand_unit_disk() {
    while (true) {
        vec3 tmp(rand_f8(-1, 1), rand_f8(-1, 1), 0);
        if (tmp.len_sq() >= 1) continue;
        return tmp;
    }
}

template <typename T>
concept intergral = std::is_integral<T>::value;

template <intergral T, int SZ>
std::array<T, SZ>* rand_perm() {
    std::array<T, SZ> ret;
    std::iota(ret.begin(), ret.end(), 0);
    for (int i = SZ - 1; i >= 0; i--) {
        int tar = rand_i4(0, i);
        std::swap(ret[tar], ret[i]);
    }
    return &ret;
}

template <intergral T>
std::vector<T>* rand_perm(const int sz) {
    std::vector<T> ret;
    std::iota(ret.begin(), ret.end(), 0);
    for (int i = sz - 1; i >= 0; i--) {
        int tar = rand_i4(0, i);
        std::swap(ret[tar], ret[i]);
    }
    return &ret;
}