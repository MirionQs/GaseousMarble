#pragma once

#include "bitmap.h"

#include <string_view>

namespace gm {

    namespace api {

        struct SpriteData {
            void* rtti;
            gm::u32 subimage_count;
            gm::api::Bitmap** bitmaps;
            gm::Position origin;
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

            Sprite(gm::api::SpriteData* data, wchar_t* name) noexcept : _data{ data }, _name{ name } {}

            std::wstring_view name() const noexcept {
                return _name;
            }

            gm::u32 subimage_count() const noexcept {
                return _data->subimage_count;
            }

            decltype(auto) origin(this auto& self) noexcept {
                return self._data->origin;
            }

            decltype(auto) bounding_box(this auto& self) noexcept {
                return self._data->bounding_box;
            }

            decltype(auto) bitmaps(this auto& self) noexcept {
                return self._data->bitmaps;
            }

            decltype(auto) bitmap(this auto& self, gm::u32 index) noexcept {
                return self._data->bitmap[index];
            }

            decltype(auto) texture_ids(this auto& self) noexcept {
                return self._data->texture_ids;
            }

            decltype(auto) texture_id(this auto& self, gm::u32 index) noexcept {
                return self._data->texture_ids[index];
            }
        };

    }

}