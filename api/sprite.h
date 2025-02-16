#pragma once

#include "bitmap.h"
#include "i_texture.h"

#include <string_view>

namespace gm {

    namespace api {

        struct SpriteData {
            void* rtti;
            gm::u32 subimage_count;
            gm::api::Bitmap** bitmaps;
            gm::Point origin;
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

            auto&& origin(this auto&& self) noexcept {
                return std::forward_like<decltype(self)>(self._data->origin);
            }

            auto&& bounding_box(this auto&& self) noexcept {
                return std::forward_like<decltype(self)>(self._data->bounding_box);
            }

            auto&& bitmap(this auto&& self, gm::u32 index) noexcept {
                assert(index < self._data->subimage_count);
                return std::forward_like<decltype(self)>(*self._data->bitmaps[index]);
            }

            auto&& texture(this auto&& self, gm::u32 index) noexcept {
                assert(index < self._data->subimage_count);
                return std::forward_like<decltype(self)>(gm::api::texture[self._data->texture_ids[index]]);
            }

            void set_texture(gm::u32 index, gm::u32 id) noexcept {
                assert(index < _data->subimage_count && id < gm::api::texture.count());
                _data->texture_ids[index] = id;
            }
        };

    }

}