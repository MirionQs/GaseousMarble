#pragma once

#include "type.h"

#ifdef GASEOUSMARBLE_EXPORTS
#define GASEOUSMARBLE_API __declspec(dllexport)
#else
#define GASEOUSMARBLE_API __declspec(dllimport)
#endif

extern "C" {

    GASEOUSMARBLE_API gm::api::real gm_font(gm::api::string sprite_path, gm::api::string glyph_path) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_width(gm::api::string text) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_height(gm::api::string text) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_draw(gm::api::real x, gm::api::real y, gm::api::string text) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_free(gm::api::real font_id) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_clear() noexcept;

    GASEOUSMARBLE_API gm::api::real gm_set_font(gm::api::real font_id) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_color(gm::api::real color) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_color2(gm::api::real color_top, gm::api::real color_bottom) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_alpha(gm::api::real alpha) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_halign(gm::api::real align) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_valign(gm::api::real align) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_align(gm::api::real halign, gm::api::real valign) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_max_line_width(gm::api::real max_width) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_letter_spacing(gm::api::real spacing) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_word_spacing(gm::api::real spacing) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_line_height(gm::api::real height) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_offset(gm::api::real x, gm::api::real y) noexcept;
    GASEOUSMARBLE_API gm::api::real gm_set_scale(gm::api::real x, gm::api::real y) noexcept;

    GASEOUSMARBLE_API gm::api::real gm_get_font() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_color_top() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_color_bottom() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_alpha() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_halign() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_valign() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_max_line_width() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_letter_spacing() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_word_spacing() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_line_height() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_offset_x() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_offset_y() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_scale_x() noexcept;
    GASEOUSMARBLE_API gm::api::real gm_get_scale_y() noexcept;

}