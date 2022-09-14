#pragma once
#include "../rtow.h"
#include "../texture.h"

class fixed_color : public texture{
    public:
        fixed_color() = default;
        fixed_color(color c) : color_val(c) {};

        virtual color value(f8 porlar, f8 azim, const pt3& p) const override{
            return color_val;
        }

        color color_val;
};
