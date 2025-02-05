#pragma once

#include "../utils/integer.h"

#include <d3dx8.h>

namespace gm {

    namespace api {

        class Texture {
            struct Size {
                gm::u32 width, height;
            };

            IDirect3DTexture8* _data;
            Size _image_size;
            Size _texture_size;
            bool _is_valid;

        public:
            Texture() = delete;
            Texture(const Texture&) = delete;
            Texture(Texture&&) = delete;

            Texture& operator=(const Texture&) = delete;
            Texture& operator=(Texture&&) = delete;

            decltype(auto) image_size(this auto& self) noexcept {
                return self._image_size;
            }

            decltype(auto) texture_size(this auto& self) noexcept {
                return self._texture_size;
            }

            decltype(auto) data(this auto& self) noexcept {
                return self._data;
            }
        };

    }

}