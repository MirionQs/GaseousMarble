#pragma once

#include <cstddef>
#include <cstdint>

namespace gm {

    namespace api {

        class Bitmap {
            void* _rtti;
            std::size_t _width, _height;
            std::uint32_t* _data;

        public:
            Bitmap() = delete;
            Bitmap(const Bitmap&) = delete;
            Bitmap(Bitmap&&) = delete;

            Bitmap& operator=(const Bitmap&) = delete;
            Bitmap& operator=(Bitmap&&) = delete;

            std::size_t width() const noexcept {
                return _width;
            }

            std::size_t height() const noexcept {
                return _height;
            }

            void set_width(std::size_t new_width) noexcept {
                _width = new_width;
            }

            void set_height(std::size_t new_height) noexcept {
                _height = new_height;
            }
        };

    }

}