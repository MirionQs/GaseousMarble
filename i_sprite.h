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
                std::int32_t origin_x, origin_y;
                std::int32_t bbox_left, bbox_top, bbox_right, bbox_bottom;
                void* masks;
                bool seperate_masks;
                std::size_t* texture_ids;
            };

            struct SpriteResource {
                SpriteData** sprites;
                wchar_t** names;
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