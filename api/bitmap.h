#pragma once

#include <cstddef>
#include <cstdint>

namespace gm {

    namespace api {

        struct BitmapSize {
            std::size_t width, height;
        };

        class Bitmap {
            void* _rtti;
            BitmapSize _size;
            std::uint32_t* _data;

        public:
            BitmapSize& size() noexcept {
                return _size;
            }

            const std::uint32_t* data() const noexcept {
                return _data;
            }
        };

    }

}