#pragma once

#include "api/i_function.h"

namespace gm {

    void draw_sprite_general(
        std::size_t sprite_id,
        std::int32_t image_index,
        std::uint32_t image_x,
        std::uint32_t image_y,
        std::size_t width,
        std::size_t height,
        double x,
        double y,
        double scale_x,
        double scale_y,
        double rotate,
        std::uint32_t color1,
        std::uint32_t color2,
        std::uint32_t color3,
        std::uint32_t color4,
        double alpha
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

    std::size_t sprite_add(
        std::string_view path,
        std::size_t image_count,
        bool remove_background,
        bool smooth_edges,
        std::int32_t origin_x,
        std::int32_t origin_y
    ) noexcept {
        using namespace gm::api;
        return function[FunctionId::sprite_add].call<
            std::size_t,
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

    void sprite_delete(std::size_t sprite_id) noexcept {
        using namespace gm::api;
        function[FunctionId::sprite_delete].call<void, Real>(sprite_id);
    }

}