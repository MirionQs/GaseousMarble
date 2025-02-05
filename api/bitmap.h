#pragma once

#include "../utils/structure.h"

namespace gm {

    namespace api {

        class Bitmap {
            void* _rtti;
            gm::Size _size;
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