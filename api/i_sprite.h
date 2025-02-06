#pragma once

#include "sprite.h"

#include <cassert>
#include <stdexcept>

namespace gm {

    namespace api {

        class ISprite {
            struct SpriteResource {
                gm::api::SpriteData** sprites;
                wchar_t** names;
                gm::u32 count;
            };

            SpriteResource* _resource;

        public:
            ISprite() noexcept :
                _resource{ reinterpret_cast<SpriteResource*>(0x00686ac8) } {};

            gm::api::Sprite operator[](gm::u32 id) const noexcept {
                assert(id < _resource->count);
                return { _resource->sprites[id], _resource->names[id] };
            }

            gm::u32 count() const noexcept {
                return _resource->count;
            }

            gm::u32 find(std::wstring_view name) const noexcept {
                for (gm::u32 id{}; id != _resource->count; ++id) {
                    if (std::wcscmp(name.data(), _resource->names[id]) == 0) {
                        return id;
                    }
                }
                return -1;
            }
        };

        inline gm::api::ISprite sprite;

    }

}