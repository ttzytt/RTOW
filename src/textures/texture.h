#pragma once
#include "../rtow.h"

class texture{
    public:
        // 击中点坐标算出来的极角和方位角
        virtual color value(f8 polar, f8 azim, const pt3& p) const = 0;
};
