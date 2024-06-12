#include "abi.h"

struct glyph_data {
	std::uint8_t tile_x, tile_y;
	std::int8_t left, top, right, bottom;
};

struct font_data {
	std::size_t sprite;
	std::size_t tile_width, tile_height;
	std::map<std::uint16_t, glyph_data> glyph;
};

struct draw_setting {
	std::size_t font{0};
	std::uint32_t color_top{0xffffff}, color_bottom{0xffffff};
	int halign{-1}, valign{-1};
	double alpha{1};
	double line_height{16};
	double max_line_width{-1};
	double letter_spacing{0}, word_spacing{0};
	double offset_x{0}, offset_y{0};
};

gm::function<void*, gm::string> get_function_pointer;
gm::function<std::size_t, gm::string, gm::real, gm::real, gm::real, gm::real, gm::real> sprite_add;
gm::function<std::size_t, gm::real> sprite_get_width;
gm::function<std::size_t, gm::real> sprite_get_height;
gm::function<void, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real> draw_sprite_general;

std::vector<font_data> gm_font_data;
draw_setting gm_draw_setting;

gm::real gm_init(gm::real ptr) {
	get_function_pointer = (void*)(gm::dword)ptr;
	sprite_add = get_function_pointer("sprite_add");
	sprite_get_width = get_function_pointer("sprite_get_width");
	sprite_get_height = get_function_pointer("sprite_get_height");
	draw_sprite_general = get_function_pointer("draw_sprite_general");
	return 0;
}

gm::real gm_font(gm::string sprite_path, gm::string glyph_path) {
	font_data data;

	std::ifstream file{glyph_path, std::ios::binary};
	std::uint8_t tile_x{0}, tile_y{0};
	while (file) {
		char buffer[6];
		file.read(buffer, 6);

		std::int8_t* i8{(std::int8_t*)buffer};
		std::uint8_t* u8{(std::uint8_t*)buffer};
		data.glyph[(u8[0] << 8) + u8[1]] = {tile_x, tile_y, i8[2], i8[3], i8[4], i8[5]};

		if (++tile_x == 0) {
			++tile_y;
		}
	}

	data.sprite = sprite_add(sprite_path, 1, 0, 0, 0, 0);
	data.tile_width = sprite_get_width(data.sprite) >> 8;
	data.tile_height = sprite_get_height(data.sprite) / (tile_y + (tile_x != 0));

	gm_font_data.emplace_back(data);

	return gm_font_data.size() - 1;
}

gm::real gm_draw(gm::real x, gm::real y, gm::string raw_text) {
	std::wstring text{gm::utf16_base64_decode(raw_text)};

	std::size_t line_count{1};
	for (auto& i : text) {
		if (i == '\n') {
			++line_count;
		}
	}

	auto& font_data{gm_font_data[gm_draw_setting.font]};
	std::vector<double> line_offset(line_count);
	if (gm_draw_setting.halign >= 0) {
		double line_width{0};
		auto p{line_offset.begin()};
		for (auto& i : text) {
			if (i == '\n') {
				line_width = std::max(line_width - gm_draw_setting.letter_spacing, 0.);
				*p++ = gm_draw_setting.halign == 0 ? -line_width / 2 : -line_width;
				line_width = 0;
			}
			else {
				line_width += font_data.glyph[i].right + gm_draw_setting.letter_spacing;
				if (i == ' ') {
					line_width += gm_draw_setting.word_spacing;
				}
			}
		}
		line_width = std::max(line_width - gm_draw_setting.letter_spacing, 0.);
		*p++ = gm_draw_setting.halign == 0 ? -line_width / 2 : -line_width;
	}

	x += gm_draw_setting.offset_x;
	y += gm_draw_setting.offset_y;
	if (gm_draw_setting.valign >= 0) {
		double text_height{gm_draw_setting.line_height * line_count};
		y -= gm_draw_setting.valign == 0 ? text_height / 2 : text_height;
	}

	auto p{line_offset.begin()};
	double draw_x{x + *p++}, draw_y{y};
	for (auto& i : text) {
		if (i == '\n') {
			draw_x = x + *p++;
			draw_y += gm_draw_setting.line_height;
		}
		else {
			auto& glyph_data{font_data.glyph[i]};
			draw_sprite_general(
				font_data.sprite,
				0,
				glyph_data.tile_x * font_data.tile_width,
				glyph_data.tile_y * font_data.tile_height,
				glyph_data.right - glyph_data.left,
				glyph_data.bottom - glyph_data.top,
				draw_x + glyph_data.left,
				draw_y + glyph_data.top,
				1,
				1,
				0,
				gm_draw_setting.color_top,
				gm_draw_setting.color_top,
				gm_draw_setting.color_bottom,
				gm_draw_setting.color_bottom,
				gm_draw_setting.alpha
			);
			draw_x += glyph_data.right + gm_draw_setting.letter_spacing;
			if (i == ' ') {
				draw_x += gm_draw_setting.word_spacing;
			}
		}
	}

	return 0;
}

gm::real gm_set_font(gm::real font) {
	gm_draw_setting.font = (std::size_t)font;
	return 0;
}

gm::real gm_set_color(gm::real color_top, gm::real color_bottom) {
	gm_draw_setting.color_top = (std::uint32_t)color_top;
	gm_draw_setting.color_bottom = (std::uint32_t)color_bottom;
	return 0;
}

gm::real gm_set_align(gm::real halign, gm::real valign) {
	gm_draw_setting.halign = halign == 0 ? 0 : halign < 0 ? -1 : 1;
	gm_draw_setting.valign = valign == 0 ? 0 : valign < 0 ? -1 : 1;
	return 0;
}

gm::real gm_set_alpha(gm::real alpha) {
	gm_draw_setting.alpha = alpha;
	return 0;
}

gm::real gm_set_line_height(gm::real height) {
	gm_draw_setting.line_height = height;
	return 0;
}

gm::real gm_set_max_line_width(gm::real width) {
	gm_draw_setting.max_line_width = width;
	return 0;
}


gm::real gm_set_letter_spacing(gm::real spacing) {
	gm_draw_setting.letter_spacing = spacing;
	return 0;
}

gm::real gm_set_word_spacing(gm::real spacing) {
	gm_draw_setting.word_spacing = spacing;
	return 0;
}

gm::real gm_set_offset(gm::real x, gm::real y) {
	gm_draw_setting.offset_x = x;
	gm_draw_setting.offset_y = y;
	return 0;
}