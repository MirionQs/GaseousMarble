#pragma once

#include "api/type.h"

#ifdef GASEOUSMARBLE_EXPORTS
#define GASEOUSMARBLE_API __declspec(dllexport)
#else
#define GASEOUSMARBLE_API __declspec(dllimport)
#endif

extern "C" {

    GASEOUSMARBLE_API gm::api::Real gm_font(gm::api::StringView sprite_path, gm::api::StringView glyph_path) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_width(gm::api::StringView text) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_height(gm::api::StringView text) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_draw(gm::api::Real x, gm::api::Real y, gm::api::StringView text) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_free(gm::api::Real font_id) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_clear() noexcept;

    GASEOUSMARBLE_API gm::api::Real gm_set_font(gm::api::Real font_id) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_color(gm::api::Real color) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_color2(gm::api::Real color_top, gm::api::Real color_bottom) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_alpha(gm::api::Real alpha) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_halign(gm::api::Real align) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_valign(gm::api::Real align) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_align(gm::api::Real halign, gm::api::Real valign) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_max_line_width(gm::api::Real max_width) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_letter_spacing(gm::api::Real spacing) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_word_spacing(gm::api::Real spacing) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_line_height(gm::api::Real height) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_offset(gm::api::Real x, gm::api::Real y) noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_set_scale(gm::api::Real x, gm::api::Real y) noexcept;

    GASEOUSMARBLE_API gm::api::Real gm_get_font() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_color_top() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_color_bottom() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_alpha() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_halign() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_valign() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_max_line_width() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_letter_spacing() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_word_spacing() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_line_height() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_offset_x() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_offset_y() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_scale_x() noexcept;
    GASEOUSMARBLE_API gm::api::Real gm_get_scale_y() noexcept;

}