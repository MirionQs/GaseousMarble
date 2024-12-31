#pragma once

#include "api.h"

namespace gm {

    class sprite_handle {
        uint32_t _id;

    public:
        sprite_handle() noexcept :
            _id{} {}

        sprite_handle(std::nullptr_t) noexcept :
            _id{} {}

        sprite_handle(uint32_t id) noexcept :
            _id{ id } {}

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(sprite_handle other) const noexcept {
            return _id == other._id;
        }

        uint32_t id() const noexcept {
            return _id;
        }
    };

    struct sprite_deleter {
        using pointer = sprite_handle;

        void operator()(sprite_handle handle) const noexcept {
            sprite_delete(handle.id());
        }
    };

}