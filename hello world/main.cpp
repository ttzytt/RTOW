#include <cmath>
#include <iostream>
#include <stdio.h>

#include "dbg.h"
  std;

int main() {
    // freopen("./hello world.ppm", "w", stdout);
    const int width = 256;
    const int height = 256;
    cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = height - 1; j >= 0; j--) {
        cerr << "\rline remaining" << j << ' ';
        for (int i = 0; i < width; i++) {
            float r = 1.0 * i / (width - 1);
            float g = 1.0 * j / (height - 1);
            float b = 0.5;

            int ir = round(255 * r);
            int ig = round(255 * g);
            int ib = round(255 * b);
            cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}