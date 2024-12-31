#pragma once

#include "function.h"

namespace gm {

    inline gm::function<void*, gm::string>
        get_function_address;
    inline gm::function<std::uint32_t, gm::string, gm::real, gm::real, gm::real, gm::real, gm::real>
        sprite_add;
    inline gm::function<void, gm::real>
        sprite_delete;
    inline gm::function<void, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real>
        draw_sprite_general;

    inline void init() noexcept {
        gm::get_function_address = reinterpret_cast<void*>(0x0064c89c);
        gm::sprite_add = gm::get_function_address("sprite_add");
        gm::sprite_delete = gm::get_function_address("sprite_delete");
        gm::draw_sprite_general = gm::get_function_address("draw_sprite_general");
    }

}