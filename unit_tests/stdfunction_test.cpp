#define __AVX__ 1
#define __AVX2__ 1
#pragma GCC target("avx,avx2,popcnt,tune=native")
#include <immintrin.h>
#include <bits/stdc++.h>

namespace with_avx {
class vec {
   public:
    vec(double x = 0, double y = 0, double z = 0, double t = 0) {
        vec_data = _mm256_set_pd(t, z, y, x);
    }
    __m256d vec_data;
};
}  // namespace with_avx

namespace without_avx {
class vec {
   public:
    vec(double x = 0, double y = 0, double z = 0, double t = 0) {
        vec_data[0] = x, vec_data[1] = y, vec_data[2] = z, vec_data[3] = t;
    }
    double vec_data[4];
};
}  // namespace without_avx

#ifdef USE_AVX
using namespace with_avx;
#else
using namespace without_avx;
#endif

vec same(vec x) { return x; }
std::function<vec(vec)> stdfunc = same;

int main() { 
    vec rand_vec(rand(), rand(), rand());
    vec ret = stdfunc(rand_vec);
    std::cout<<(double)ret.vec_data[0];
}