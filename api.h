#pragma once

#include "function.h"

namespace gm {

    namespace api {

        inline gm::api::function<void*, gm::api::string>
        get_function_address;
        inline gm::api::function<std::size_t, gm::api::string, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real>
        sprite_add;
        inline gm::api::function<void, gm::api::real>
        sprite_delete;
        inline gm::api::function<void, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real, gm::api::real>
        draw_sprite_general;

    inline void init() noexcept {
            gm::api::get_function_address = reinterpret_cast<void*>(0x0064c89c);
            gm::api::sprite_add = gm::api::get_function_address("sprite_add");
            gm::api::sprite_delete = gm::api::get_function_address("sprite_delete");
            gm::api::draw_sprite_general = gm::api::get_function_address("draw_sprite_general");
    }

}