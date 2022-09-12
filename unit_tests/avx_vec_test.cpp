#include <functional>

#include "../src/rtow.h"
using namespace std;

const int TEST_REP = 50;

int operator<=>(avx2::vec3 avx, navx2::vec3 norm) {
    if (avx[0] == norm[0] && avx[1] == norm[1] && avx[2] == norm[2]) {
        return 0;
    }
    if (avx[0] < norm[0] || avx[1] < norm[1] || avx[2] < norm[2]) {
        return -1;
    }
    if (avx[0] > norm[0] || avx[1] > norm[1] || avx[2] > norm[2]) {
        return 1;
    }
}

bool comp_and_out(avx2::vec3 avx, navx2::vec3 norm, string name) {
    if ((avx <=> norm) != 0) {
        cerr << "fail on " + name << endl;
        cerr << "avx: " << avx << " norm " << norm << endl;
        return false;
    }
    cerr << "succ on " + name << endl;
    return true;
}

bool comp_and_out(f8 avx, f8 norm, string name) {
    if ((avx <=> norm) != 0) {
        cerr << "fail on " + name << endl;
        cerr << "avx: " << avx << " norm " << norm << endl;
        return false;
    }
    cerr << "succ on " + name << endl;
    return true;
}

int main() {
    avx2::vec3 avx;
    navx2::vec3 norm;
    for (int i = 1; i <= TEST_REP; i++) {
        norm = rand_unit_vec();
        avx = norm;
        navx2::vec3 tmp = rand_unit_vec();
        if (!comp_and_out(avx.refract(tmp, 1.5), norm.refract(tmp, 1.5),
                          "refract")) {
            break;
        }
    }

    for (int i = 1; i <= TEST_REP; i++) {
        norm = norm.rand();
        avx = norm;
        if (!comp_and_out(avx + avx, norm + norm, "add")) {
            break;
        }
    }

    for (int i = 1; i <= TEST_REP; i++) {
        norm = norm.rand();
        avx = norm;
        if (!comp_and_out(avx - avx, norm - norm, "sub")) {
            break;
        }
    }

    for (int i = 1; i <= TEST_REP; i++) {
        norm = norm.rand();
        navx2::vec3 tmp = norm.rand();
        avx = norm;
        if (!comp_and_out(cross(avx, tmp), cross(norm, tmp), "cross")) {
            break;
        }
    }

    for (int i = 1; i <= TEST_REP; i++) {
        norm = norm.rand();
        navx2::vec3 tmp = norm.rand();
        avx = norm;
        if (!comp_and_out(dot(avx, tmp), dot(norm, tmp), "dot")) {
            break;
        }
    }

    for (int i = 1; i <= TEST_REP; i++) {
        norm = norm.rand();
        avx = norm;
        if (!comp_and_out(avx.len_sq(), norm.len_sq(), "len_sq")) {
            break;
        }
    }
}