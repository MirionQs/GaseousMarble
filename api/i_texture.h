#pragma once

#include "../utils/int.h"

#include <d3dx8.h>

namespace gm {

    namespace api {

        class ITexture {
            struct TextureData {
                IDirect3DTexture8* texture;
                gm::u32 image_width, image_height;
                gm::u32 texture_width, texture_height;
                bool is_valid;
            };

            TextureData** _textures;
            gm::u32* _count;

        public:
            ITexture() noexcept :
                _textures{ reinterpret_cast<TextureData**>(0x0085b3c4) },
                _count{ reinterpret_cast<gm::u32*>(0x006886f0) } {};

            //const Texture& operator[](gm::u32 id) const noexcept {
            //    return (*_textures)[id];
            //}

            gm::u32 count() const noexcept {
                return *_count;
            }
        };

        inline gm::api::ITexture texture;

    }

}