#pragma once
#include <cmath>
#include <iostream>

#include "rtow.h"

inline color gamma_correct(const color& pixel, f8 gamma_val) {
    return color(powf64(pixel[0], 1.0 / gamma_val), powf64(pixel[1], 1.0 / gamma_val),
                 powf64(pixel[2], 1.0 / gamma_val));
}

void write_color(std::ostream& out, const color& pixel) {
    color&& corrected = gamma_correct(pixel, 2.2);
#ifdef OUT
    out << clamp(round(corrected[0] * MX_COLOR), 0, MX_COLOR) << ' ' << clamp(round(corrected[1] * MX_COLOR), 0, MX_COLOR)
        << ' ' << clamp(round(corrected[2] * MX_COLOR), 0, MX_COLOR) << '\n';
#endif
}

void write_color(std::ostream& out, const color& pixel, int samples_per_pix) {
    f8 scale = 1.0 / samples_per_pix;
    write_color(out, pixel * scale); // gamma 2.2
}