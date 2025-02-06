#pragma once

#include "../utils/structure.h"

#include <utility>

namespace gm {

    namespace api {

        class Bitmap {
            void* _rtti;
            gm::Size _size;
            void* _data;

        public:
            Bitmap() = delete;

            auto&& size(this auto&& self) noexcept {
                return std::forward_like<decltype(self)>(self._size);
            }
        };

    }

}