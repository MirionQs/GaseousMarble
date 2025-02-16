#pragma once

#include "function.h"

namespace gm {

    class SpriteHandle {
        gm::u32 _id;

    public:
        SpriteHandle() noexcept :
            _id{} {}

        SpriteHandle(std::nullptr_t) noexcept :
            _id{} {}

        SpriteHandle(gm::u32 id) noexcept :
            _id{ id } {}

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(SpriteHandle other) const noexcept {
            return _id == other._id;
        }

        gm::u32 id() const noexcept {
            return _id;
        }
    };

    struct SpriteDeleter {
        using pointer = gm::SpriteHandle;

        void operator()(gm::SpriteHandle handle) const noexcept {
            gm::sprite_delete(handle.id());
        }
    };

}