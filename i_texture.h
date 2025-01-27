#pragma once

#include <cstddef>

namespace gm {

    namespace api {

        class ITexture {
            struct TextureData {
                void* texture; // IDirect3DTexture8
                std::size_t image_width, image_height;
                std::size_t texture_width, texture_height;
                bool is_valid;
            };

            TextureData** _textures;
            std::size_t* _count;

        public:
            ITexture() noexcept :
                _textures{ reinterpret_cast<TextureData**>(0x0085b3c4) },
                _count{ reinterpret_cast<std::size_t*>(0x006886f0) } {};
        };

        inline gm::api::ITexture texture;

    }

}