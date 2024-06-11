#include "pch.h"
#include "abi.h"

struct font_data {
	struct glyph_data {
		std::uint8_t tile_x, tile_y;
		std::int8_t left, top, right, bottom;
	};

	std::size_t sprite;
	std::uint32_t tile_width, tile_height;
	std::map<std::uint16_t, glyph_data> glyph;
};

struct draw_setting {
	std::size_t font;
	std::uint32_t color_top;
	std::uint32_t color_bottom;
	double alpha;
	std::int8_t halign;
	std::int8_t valign;
	double letter_spacing;
	double line_height;
	double offset_x;
	double offset_y;
};

static std::vector<font_data> gm_font_data;
static draw_setting gm_draw_setting{0, 0xffffff, 0xffffff, 1, -1, -1, 0, 16, 0, 0};

gm::real gm_init(gm::real ptr) {
	gm::init(ptr);
	return 0;
}

gm::real gm_font(gm::string sprite_path, gm::string glyph_path) {
	font_data data;

	std::ifstream file{glyph_path, std::ios::binary};
	char buffer[6];
	std::uint8_t tile_x{0}, tile_y{0};
	while (file) {
		file.read(buffer, 6);
		std::uint16_t unicode{(std::uint16_t)(((std::uint8_t)buffer[0] << 8) + (std::uint8_t)buffer[1])};
		data.glyph[unicode] = {tile_x, tile_y, (std::int8_t)buffer[2], (std::int8_t)buffer[3], (std::int8_t)buffer[4], (std::int8_t)buffer[5]};
		if (++tile_x == 0) {
			++tile_y;
		}
	}

	data.sprite = (std::size_t)gm::sprite_add(sprite_path, 1, 0, 0, 0, 0);
	data.tile_width = (std::uint32_t)gm::sprite_get_width(data.sprite) >> 8;
	data.tile_height = (std::uint32_t)gm::sprite_get_height(data.sprite) / (tile_y + (tile_x != 0));

	gm_font_data.emplace_back(data);

	return gm_font_data.size() - 1;
}

gm::real gm_draw(gm::real x, gm::real y, gm::string text) {
	auto& font_data{gm_font_data[gm_draw_setting.font]};

	std::vector<double> line_offset;
	double line_width{0}, text_width{0};
	for (const char* i{text}; true; ++i) {
		if (*i == '\n' || *i == '\0') {
			line_width = std::max(line_width - gm_draw_setting.letter_spacing, 0.);
			text_width = std::max(text_width, line_width);
			if (gm_draw_setting.halign < 0) {
				line_offset.emplace_back(0);
			}
			else if (gm_draw_setting.halign == 0) {
				line_offset.emplace_back(-line_width / 2);
			}
			else {
				line_offset.emplace_back(-line_width);
			}
			line_width = 0;
			if (*i == '\0') {
				break;
			}
		}
		else {
			line_width += font_data.glyph[*i].right + gm_draw_setting.letter_spacing;
		}
	}

	x += gm_draw_setting.offset_x;
	y += gm_draw_setting.offset_y;
	if (gm_draw_setting.valign >= 0) {
		double text_height{gm_draw_setting.line_height * line_offset.size()};
		if (gm_draw_setting.valign == 0) {
			y -= text_height / 2;
		}
		else {
			y -= text_height;
		}
	}

	auto off{line_offset.begin()};
	double draw_x{x + *off++}, draw_y{y};
	for (const char* i{text}; *i != '\0'; ++i) {
		if (*i == '\n') {
			draw_x = x + *off++;
			draw_y += gm_draw_setting.line_height;
		}
		else {
			auto& glyph_data{font_data.glyph[*i]};
			gm::draw_sprite_general(
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

gm::real gm_set_alpha(gm::real alpha) {
	gm_draw_setting.alpha = alpha;
	return 0;
}

gm::real gm_set_align(gm::real halign, gm::real valign) {
	gm_draw_setting.halign = halign == 0 ? 0 : halign < 0 ? -1 : 1;
	gm_draw_setting.valign = valign == 0 ? 0 : valign < 0 ? -1 : 1;
	return 0;
}

gm::real gm_set_letter_spacing(gm::real spacing) {
	gm_draw_setting.letter_spacing = spacing;
	return 0;
}

gm::real gm_set_line_height(gm::real height) {
	gm_draw_setting.line_height = height;
	return 0;
}

gm::real gm_set_offset(gm::real x, gm::real y) {
	gm_draw_setting.offset_x = x;
	gm_draw_setting.offset_y = y;
	return 0;
}
