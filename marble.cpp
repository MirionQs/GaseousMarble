#include "marble.h"

gm::draw_system draw;

gm::real gm_init(gm::real ptr) {
	if (ptr <= 0) {
		return false;
	}
	draw = {(void*)(uintptr_t)ptr};
	return true;
}

gm::real gm_font(gm::string sprite_path, gm::string glyph_path) {
	return draw.add_font(sprite_path, glyph_path);
}

gm::real gm_draw(gm::real x, gm::real y, gm::string text) {
	return draw.draw_text(x, y, gm::utf8_to_ucs2(text));
}

gm::real gm_free(gm::real font_id) {
	return draw.remove_font((size_t)font_id);
}

gm::real gm_set_font(gm::real font_id) {
	return draw.set_font((size_t)font_id);
}

gm::real gm_set_color(gm::real color) {
	return gm_set_color2(color, color);
}

gm::real gm_set_color2(gm::real color_top, gm::real color_bottom) {
	draw.setting().color_top = (uint32_t)color_top;
	draw.setting().color_bottom = (uint32_t)color_bottom;
	return true;
}

gm::real gm_set_alpha(gm::real alpha) {
	if (alpha < 0 || alpha > 1) {
		return false;
	}
	draw.setting().alpha = alpha;
	return true;
}

gm::real gm_set_halign(gm::real align) {
	draw.setting().halign = align == 0 ? 0 : align < 0 ? -1 : 1;
	return true;
}

gm::real gm_set_valign(gm::real align) {
	draw.setting().valign = align == 0 ? 0 : align < 0 ? -1 : 1;
	return true;
}

gm::real gm_set_align(gm::real halign, gm::real valign) {
	gm_set_halign(halign);
	gm_set_valign(valign);
	return true;
}

gm::real gm_set_max_line_width(gm::real max_width) {
	if (max_width <= 0) {
		return false;
	}
	draw.setting().max_line_width = abs(max_width);
	return true;
}

gm::real gm_set_letter_spacing(gm::real spacing) {
	draw.setting().letter_spacing = spacing;
	return true;
}

gm::real gm_set_word_spacing(gm::real spacing) {
	draw.setting().word_spacing = spacing;
	return true;
}

gm::real gm_set_line_height(gm::real height) {
	if (height <= 0) {
		return false;
	}
	draw.setting().line_height = height;
	return true;
}

gm::real gm_set_offset(gm::real x, gm::real y) {
	draw.setting().offset_x = x;
	draw.setting().offset_y = y;
	return true;
}

gm::real gm_set_scale(gm::real x, gm::real y) {
	if (x <= 0 || y <= 0) {
		return false;
	}
	draw.setting().scale_x = x;
	draw.setting().scale_y = y;
	return true;
}

gm::real gm_get_font() {
	return draw.setting().font_id;
}

gm::real gm_get_color_top() {
	return draw.setting().color_top;
}

gm::real gm_get_color_bottom() {
	return draw.setting().color_bottom;
}

gm::real gm_get_alpha() {
	return draw.setting().alpha;
}

gm::real gm_get_halign() {
	return draw.setting().halign;
}

gm::real gm_get_valign() {
	return draw.setting().valign;
}

gm::real gm_get_max_line_width() {
	return draw.setting().max_line_width;
}

gm::real gm_get_letter_spacing() {
	return draw.setting().letter_spacing;
}

gm::real gm_get_word_spacing() {
	return draw.setting().word_spacing;
}

gm::real gm_get_line_height() {
	return draw.setting().line_height;
}

gm::real gm_get_offset_x() {
	return draw.setting().offset_x;
}

gm::real gm_get_offset_y() {
	return draw.setting().offset_y;
}

gm::real gm_get_scale_x() {
	return draw.setting().scale_x;
}

gm::real gm_get_scale_y() {
	return draw.setting().scale_y;
}
