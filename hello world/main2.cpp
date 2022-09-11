#include <stdio.h>

#include <cmath>
#include <iostream>

#include "../src/color.h"
#include "../src/vec3.h"
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
            color pixel(1.0 * i / (width - 1), 1.0 * j / (height - 1), 0.25);
            write_color(cout, pixel);
        }
    }
}