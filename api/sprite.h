#pragma once

#include "bitmap.h"

#include <string_view>

namespace gm {

    namespace api {

        struct SpriteData {
            void* rtti;
            gm::u32 subimage_count;
            gm::api::Bitmap** bitmaps;
            gm::i32 origin_x, origin_y;
            gm::BoundingBox bounding_box;
            void* masks;
            bool seperate_masks;
            gm::u32* texture_ids;
        };

        class Sprite {
            gm::api::SpriteData* _data;
            std::wstring_view _name;

        public:
            Sprite() = delete;
            Sprite(const Sprite&) = delete;
            Sprite(Sprite&&) = delete;

            Sprite& operator=(const Sprite&) = delete;
            Sprite& operator=(Sprite&&) = delete;

            Sprite(gm::api::SpriteData* data, wchar_t* name) noexcept : _data{ data }, _name{ name } {}

            std::wstring_view name() const noexcept {
                return _name;
            }

            gm::u32 subimage_count() const noexcept {
                return _data->subimage_count;
            }

            decltype(auto) bounding_box(this auto& self) noexcept {
                return self.bounding_box;
            }
        };

    }

}