#pragma once

#include "i_function.h"

namespace gm {

    class sprite_handle {
        std::size_t _id;

    public:
        sprite_handle() noexcept :
            _id{} {}

        sprite_handle(std::nullptr_t) noexcept :
            _id{} {}

        sprite_handle(std::size_t id) noexcept :
            _id{ id } {}

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(sprite_handle other) const noexcept {
            return _id == other._id;
        }

        std::size_t id() const noexcept {
            return _id;
        }
    };

    struct sprite_deleter {
        using pointer = gm::sprite_handle;

        void operator()(gm::sprite_handle handle) const noexcept {
            gm::api::function.sprite_delete(handle.id());
        }
    };

}