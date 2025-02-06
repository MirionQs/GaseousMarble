#pragma once

#include "../utils/structure.h"

#include <d3dx8.h>

namespace gm {

    namespace api {

        class Texture {
            IDirect3DTexture8* _data;
            gm::Size _image_size;
            gm::Size _texture_size;
            bool _is_valid;

        public:
            Texture() = delete;

            auto&& image_size(this auto& self) noexcept {
                return std::forward_like<decltype(self)>(self._image_size);
            }

            auto&& texture_size(this auto& self) noexcept {
                return std::forward_like<decltype(self)>(self._texture_size);
            }

            auto&& data(this auto& self) noexcept {
                return std::forward_like<decltype(self)>(self._data);
            }
        };

    }

}