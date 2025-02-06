#pragma once

#include "texture.h"

#include <cassert>

namespace gm {

    namespace api {

        class ITexture {
            gm::api::Texture** _textures;
            gm::u32* _count;

        public:
            ITexture() noexcept :
                _textures{ reinterpret_cast<gm::api::Texture**>(0x0085b3c4) },
                _count{ reinterpret_cast<gm::u32*>(0x006886f0) } {};

            const gm::api::Texture& operator[](gm::u32 id) const noexcept {
                assert(id < *_count);
                return (*_textures)[id];
            }

            gm::u32 count() const noexcept {
                return *_count;
            }
        };

        inline gm::api::ITexture texture;

    }

}