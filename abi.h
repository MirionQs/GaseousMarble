#pragma once

#include "pch.h"
#include "gm.h"

#ifdef GASEOUSMARBLE_EXPORTS
#define GASEOUSMARBLE_API extern "C" __declspec(dllexport)
#else
#define GASEOUSMARBLE_API extern "C" __declspec(dllimport)
#endif

GASEOUSMARBLE_API gm::real gm_init(gm::real ptr);
GASEOUSMARBLE_API gm::real gm_font(gm::string sprite_path, gm::string glyph_path);
GASEOUSMARBLE_API gm::real gm_draw(gm::real x, gm::real y, gm::string raw_text);
GASEOUSMARBLE_API gm::real gm_set_font(gm::real font);
GASEOUSMARBLE_API gm::real gm_set_color(gm::real color_top, gm::real color_bottom);
GASEOUSMARBLE_API gm::real gm_set_alpha(gm::real alpha);
GASEOUSMARBLE_API gm::real gm_set_align(gm::real halign, gm::real valign);
GASEOUSMARBLE_API gm::real gm_set_letter_spacing(gm::real spacing);
GASEOUSMARBLE_API gm::real gm_set_line_height(gm::real height);
GASEOUSMARBLE_API gm::real gm_set_offset(gm::real x, gm::real y);