#pragma once

#include "sprite.h"

namespace gm {

    namespace api {

        class ISprite {
            struct SpriteResource {
                SpriteData** sprites;
                wchar_t** names;
                std::size_t count;
            };

            SpriteResource* _resource;

        public:
            ISprite() noexcept :
                _resource{ reinterpret_cast<SpriteResource*>(0x00686ac8) } {};

            const Sprite& operator[](std::size_t id) const noexcept {
                return { _resource->sprites[id], _resource->names[id] };
            }

            std::size_t count() const noexcept {
                return _resource->count;
            }
        };

        inline gm::api::ISprite sprite;

    }

}