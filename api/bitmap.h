#pragma once

#include "../utils/int.h"

namespace gm {

    namespace api {

        class Bitmap {
            struct Size {
                gm::u32 width, height;
            };

            void* _rtti;
            Size _size;
            void* _data;

        public:
            decltype(auto) size(this auto& self) noexcept {
                return self._size;
            }
        };

    }

}