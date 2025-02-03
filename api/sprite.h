#pragma once

#include "bitmap.h"

#include <string_view>

namespace gm {

    namespace api {

        struct SpriteBoundingBox {
            std::int32_t left, top, right, bottom;
        };

        struct SpriteData {
            void* rtti;
            std::size_t subimage_count;
            Bitmap** bitmaps;
            std::int32_t origin_x, origin_y;
            SpriteBoundingBox bounding_box;
            void* masks;
            bool seperate_masks;
            std::size_t* texture_ids;
        };

        class Sprite {
            SpriteData* _data;
            std::wstring_view _name;

        public:
            Sprite() = delete;
            Sprite(const Sprite&) = delete;
            Sprite(Sprite&&) = delete;

            Sprite& operator=(const Sprite&) = delete;
            Sprite& operator=(Sprite&&) = delete;

            Sprite(SpriteData* data, wchar_t* name) noexcept : _data{ data }, _name{ name } {}

            std::wstring_view name() const noexcept {
                return _name;
            }

            std::size_t subimage_count() const noexcept {
                return _data->subimage_count;
            }


        };

    }

}