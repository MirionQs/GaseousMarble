#pragma once

#include "i_function.h"

namespace gm {

    class SpriteHandle {
        std::size_t _id;

    public:
        SpriteHandle() noexcept :
            _id{} {}

        SpriteHandle(std::nullptr_t) noexcept :
            _id{} {}

        SpriteHandle(std::size_t id) noexcept :
            _id{ id } {}

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(SpriteHandle other) const noexcept {
            return _id == other._id;
        }

        std::size_t id() const noexcept {
            return _id;
        }
    };

    struct SpriteDeleter {
        using pointer = gm::SpriteHandle;

        void operator()(gm::SpriteHandle handle) const noexcept {
            gm::api::function.sprite_delete(handle.id());
        }
    };

}