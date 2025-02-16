#pragma once

#include "integer.h"

namespace gm {

    struct Size {
        gm::u32 width, height;
    };

    struct Point {
        gm::i32 x, y;
    };

    struct BoundingBox {
        gm::i32 left, top, right, bottom;
    };
    
}