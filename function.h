#pragma once

#include "api/i_function.h"

namespace gm {

    void draw_sprite_general(
        gm::u32 sprite_id,
        gm::i32 image_index, // -1 indicates the current subimage
        gm::u32 image_x,
        gm::u32 image_y,
        gm::u32 width,
        gm::u32 height,
        gm::f64 x,
        gm::f64 y,
        gm::f64 scale_x,
        gm::f64 scale_y,
        gm::f64 rotate,
        gm::u32 color1,
        gm::u32 color2,
        gm::u32 color3,
        gm::u32 color4,
        gm::f64 alpha
    ) noexcept {
        using namespace gm::api;
        function[FunctionId::draw_sprite_general].call<
            void,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real,
            Real
        >(
            sprite_id,
            image_index,
            image_x,
            image_y,
            width,
            height,
            x,
            y,
            scale_x,
            scale_y,
            rotate,
            color1,
            color2,
            color3,
            color4,
            alpha
        );
    }

    gm::u32 sprite_add(
        std::string_view path,
        gm::u32 image_count,
        bool remove_background,
        bool smooth_edges,
        gm::i32 origin_x,
        gm::i32 origin_y
    ) noexcept {
        using namespace gm::api;
        return function[FunctionId::sprite_add].call<
            u32,
            String,
            Real,
            Real,
            Real,
            Real,
            Real
        >(
            path,
            image_count,
            remove_background,
            smooth_edges,
            origin_x,
            origin_y
        );
    }

    void sprite_delete(gm::u32 sprite_id) noexcept {
        using namespace gm::api;
        function[FunctionId::sprite_delete].call<void, Real>(sprite_id);
    }

}