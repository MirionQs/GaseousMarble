#pragma once

#include "../utils/integer.h"

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
            Bitmap() = delete;
            Bitmap(const Bitmap&) = delete;
            Bitmap(Bitmap&&) = delete;

            Bitmap& operator=(const Bitmap&) = delete;
            Bitmap& operator=(Bitmap&&) = delete;

            decltype(auto) size(this auto& self) noexcept {
                return self._size;
            }
        };

    }

}