#pragma once

#include "texture.h"

namespace gm {

    namespace api {

        class ITexture {
            Texture** _textures;
            gm::u32* _count;

        public:
            ITexture() noexcept :
                _textures{ reinterpret_cast<Texture**>(0x0085b3c4) },
                _count{ reinterpret_cast<gm::u32*>(0x006886f0) } {};

            const Texture& operator[](gm::u32 id) const noexcept {
                return (*_textures)[id];
            }

            gm::u32 count() const noexcept {
                return *_count;
            }
        };

        inline gm::api::ITexture texture;

    }

}