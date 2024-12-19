#pragma once

#include "api.h"

#ifdef GASEOUSMARBLE_EXPORTS
#define GASEOUSMARBLE_API __declspec(dllexport)
#else
#define GASEOUSMARBLE_API __declspec(dllimport)
#endif

extern "C" {

    GASEOUSMARBLE_API gm::real gm_font(gm::string sprite_path, gm::string glyph_path) noexcept;
    GASEOUSMARBLE_API gm::real gm_width(gm::string text) noexcept;
    GASEOUSMARBLE_API gm::real gm_height(gm::string text) noexcept;
    GASEOUSMARBLE_API gm::real gm_draw(gm::real x, gm::real y, gm::string text) noexcept;
    GASEOUSMARBLE_API gm::real gm_free(gm::real font_id) noexcept;
    GASEOUSMARBLE_API gm::real gm_clear() noexcept;

    GASEOUSMARBLE_API gm::real gm_set_font(gm::real font_id) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_color(gm::real color) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_color2(gm::real color_top, gm::real color_bottom) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_alpha(gm::real alpha) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_halign(gm::real align) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_valign(gm::real align) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_align(gm::real halign, gm::real valign) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_max_line_width(gm::real max_width) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_letter_spacing(gm::real spacing) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_word_spacing(gm::real spacing) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_line_height(gm::real height) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_offset(gm::real x, gm::real y) noexcept;
    GASEOUSMARBLE_API gm::real gm_set_scale(gm::real x, gm::real y) noexcept;

    GASEOUSMARBLE_API gm::real gm_get_font() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_color_top() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_color_bottom() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_alpha() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_halign() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_valign() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_max_line_width() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_letter_spacing() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_word_spacing() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_line_height() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_offset_x() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_offset_y() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_scale_x() noexcept;
    GASEOUSMARBLE_API gm::real gm_get_scale_y() noexcept;

}