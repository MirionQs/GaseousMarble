#pragma once

#include <cstddef>
#include <cstdint>

namespace gm {

    namespace api {

        class ISprite {
            struct BitmapData {
                void* rtti;
                std::size_t width, height;
                char* data;
            };

            struct SpriteData {
                void* rtti;
                std::size_t size;
                BitmapData** bitmaps;
                std::int32_t x, y;
                std::int32_t left, top, right, bottom;
                void* masks;
                bool seperate_masks;
                std::size_t* texture_ids;
            };

            struct SpriteResource {
                SpriteData** sprites;
                char** names;
                std::size_t size;
            };

            SpriteResource* _resource;

        public:
            ISprite() noexcept :
                _resource{ reinterpret_cast<SpriteResource*>(0x00686ac8) } {};
        };

        inline gm::api::ISprite sprite;

    }

}