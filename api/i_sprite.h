#pragma once

#include "sprite.h"

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

            const gm::api::Sprite& operator[](gm::u32 id) const noexcept {
                return { _resource->sprites[id], _resource->names[id] };
            }

            gm::u32 count() const noexcept {
                return _resource->count;
            }
        };

        inline gm::api::ISprite sprite;

    }

}