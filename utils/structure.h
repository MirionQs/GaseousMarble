#pragma once

#include "integer.h"

namespace gm {

    struct Size {
        gm::u32 width, height;
    };

    struct BoundingBox {
        gm::i32 left, top, right, bottom;
    };

}