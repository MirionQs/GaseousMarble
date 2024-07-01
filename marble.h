#pragma once

#include "draw.h"

#ifdef GASEOUSMARBLE_EXPORTS
#define GASEOUSMARBLE_API extern "C" __declspec(dllexport)
#else
#define GASEOUSMARBLE_API extern "C" __declspec(dllimport)
#endif

GASEOUSMARBLE_API gm::real gm_init(gm::real ptr);
GASEOUSMARBLE_API gm::real gm_font(gm::string sprite_path, gm::string glyph_path);
GASEOUSMARBLE_API gm::real gm_draw(gm::real x, gm::real y, gm::string text);
GASEOUSMARBLE_API gm::real gm_free(gm::real font_id);

GASEOUSMARBLE_API gm::real gm_set_font(gm::real font_id);
GASEOUSMARBLE_API gm::real gm_set_color(gm::real color_top, gm::real color_bottom);
GASEOUSMARBLE_API gm::real gm_set_alpha(gm::real alpha);
GASEOUSMARBLE_API gm::real gm_set_align(gm::real halign, gm::real valign);
GASEOUSMARBLE_API gm::real gm_set_max_line_width(gm::real max_width);
GASEOUSMARBLE_API gm::real gm_set_letter_spacing(gm::real spacing);
GASEOUSMARBLE_API gm::real gm_set_word_spacing(gm::real spacing);
GASEOUSMARBLE_API gm::real gm_set_line_height(gm::real multiplier);
GASEOUSMARBLE_API gm::real gm_set_offset(gm::real x, gm::real y);
GASEOUSMARBLE_API gm::real gm_set_scale(gm::real x, gm::real y);

GASEOUSMARBLE_API gm::real gm_get_font();
GASEOUSMARBLE_API gm::real gm_get_color_top();
GASEOUSMARBLE_API gm::real gm_get_color_bottom();
GASEOUSMARBLE_API gm::real gm_get_alpha();
GASEOUSMARBLE_API gm::real gm_get_halign();
GASEOUSMARBLE_API gm::real gm_get_valign();
GASEOUSMARBLE_API gm::real gm_get_max_line_width();
GASEOUSMARBLE_API gm::real gm_get_letter_spacing();
GASEOUSMARBLE_API gm::real gm_get_word_spacing();
GASEOUSMARBLE_API gm::real gm_get_line_height();
GASEOUSMARBLE_API gm::real gm_get_offset_x();
GASEOUSMARBLE_API gm::real gm_get_offset_y();
GASEOUSMARBLE_API gm::real gm_get_scale_x();
GASEOUSMARBLE_API gm::real gm_get_scale_y();