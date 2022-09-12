#include "rtow.h"

class texture{
    public:
        // 极角和方位角算出来的球面上的坐标
        virtual color value(f8 virti, f8 hori, const pt3& p) const = 0;
};

class fixed_color : public texture{
    public:
        fixed_color() = default;
        fixed_color(color c) : color_val(c) {};

        virtual color value(f8 virti, f8 hori, const pt3& p) const override{
            return color_val;
        }

        color color_val;
};
