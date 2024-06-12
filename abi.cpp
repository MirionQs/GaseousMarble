#include "abi.h"

struct glyph_data {
	std::uint8_t tile_x, tile_y;
	std::int8_t left, top, right, bottom;
};

struct font_data {
	std::size_t sprite;
	std::size_t tile_width, tile_height;
	std::uint8_t size;
	std::map<char16_t, glyph_data> glyph;
};

struct draw_setting {
	bool utf16_base64{true};
	std::size_t font{0};
	std::uint32_t color_top{0xffffff}, color_bottom{0xffffff};
	double alpha{1};
	int halign{-1}, valign{-1};
	double max_line_width{0};
	double letter_spacing{0}, word_spacing{0};
	double line_height{1};
	double offset_x{0}, offset_y{0};
	double scale_x{1}, scale_y{1};
};

std::vector<font_data> font_list;
draw_setting setting;

gm::function<void*, gm::string> get_function_pointer;
gm::function<std::size_t, gm::string, gm::real, gm::real, gm::real, gm::real, gm::real> sprite_add;
gm::function<std::size_t, gm::real> sprite_get_width;
gm::function<std::size_t, gm::real> sprite_get_height;
gm::function<void, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real> draw_sprite_general;

void draw_character(font_data& font, char16_t ch, double x, double y) {
	glyph_data& glyph{font.glyph[ch]};
	draw_sprite_general(
		font.sprite,
		0,
		glyph.tile_x * font.tile_width,
		glyph.tile_y * font.tile_height,
		glyph.right - glyph.left,
		glyph.bottom - glyph.top,
		x + glyph.left * setting.scale_x,
		y + glyph.top * setting.scale_y,
		setting.scale_x,
		setting.scale_y,
		0,
		setting.color_top,
		setting.color_top,
		setting.color_bottom,
		setting.color_bottom,
		setting.alpha
	);
}

void draw_line(font_data& font, std::u16string_view line, double x, double y) {
	double letter_spacing{setting.letter_spacing * setting.scale_x};
	double word_spacing{setting.word_spacing * setting.scale_x};
	for (char16_t ch : line) {
		draw_character(font, ch, x, y);
		x += font.glyph[ch].right * setting.scale_x + letter_spacing;
		if (ch == ' ') {
			x += word_spacing;
		}
	}
}

void draw_line_ralign(font_data& font, std::u16string_view line, double x, double y) {
	double letter_spacing{setting.letter_spacing * setting.scale_x};
	double word_spacing{setting.word_spacing * setting.scale_x};
	for (char16_t ch : line | std::views::reverse) {
		x -= font.glyph[ch].right * setting.scale_x;
		draw_character(font, ch, x, y);
		x -= letter_spacing;
		if (ch == ' ') {
			x -= word_spacing;
		}
	}
}

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
	char header[4];
	file.read(header, 4);
	if (header[0] != 'g' || header[1] != 'l' || header[2] != 'y') {
		return 0;
	}
	data.size = (std::uint8_t)header[3];

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

	data.sprite = sprite_add(sprite_path, 1, false, false, 0, 0);
	data.tile_width = sprite_get_width(data.sprite) >> 8;
	data.tile_height = sprite_get_height(data.sprite) / (tile_y + (tile_x != 0));

	font_list.emplace_back(data);

	return font_list.size() - 1;
}

gm::real gm_draw(gm::real x, gm::real y, gm::string raw_text) {
	std::u16string text;
	if (setting.utf16_base64) {
		text = gm::utf16_base64_decode(raw_text);
	}
	else {
		for (auto p{raw_text}; *p != 0; ++p) {
			text.push_back(*p);
		}
	}

	auto& font{font_list[setting.font]};
	std::vector<std::u16string_view> line;
	std::vector<double> offset;
	if (setting.halign != 0 && setting.max_line_width == 0) {
		for (auto&& i : text | std::views::split('\n')) {
			line.emplace_back(i);
		}
	}
	else {
		double max_line_width{setting.max_line_width * setting.scale_x};
		double letter_spacing{setting.letter_spacing * setting.scale_x};
		double word_spacing{setting.word_spacing * setting.scale_x};

		double line_width{0};
		auto begin{text.begin()}, end{text.end()};
		for (auto p{begin}; p != end; ++p) {
			if (*p == '\n') {
				offset.emplace_back((letter_spacing - line_width) / 2);
				line.emplace_back(begin, p);
				line_width = 0;
				begin = p + 1;
			}
			else {
				double char_width{font.glyph[*p].right * setting.scale_x + letter_spacing};
				if (*p == ' ') {
					char_width += word_spacing;
				}
				if (max_line_width == 0 || line_width + char_width <= max_line_width) {
					line_width += char_width;
				}
				else {
					offset.emplace_back((letter_spacing - line_width) / 2);
					line.emplace_back(begin, p);
					line_width = char_width;
					begin = p;
				}
			}
		}
		offset.emplace_back((letter_spacing - line_width) / 2);
		line.emplace_back(begin, end);
	}

	double line_height{gm::pt_to_px(font.size) * setting.line_height * setting.scale_y};
	x += setting.offset_x * setting.scale_x;
	y += setting.offset_y * setting.scale_y;
	if (setting.valign >= 0) {
		double text_height{line_height * line.size()};
		y -= setting.valign == 0 ? text_height / 2 : text_height;
	}

	if (setting.halign < 0) {
		for (auto& i : line) {
			draw_line(font, i, x, y);
			y += line_height;
		}
	}
	else if (setting.halign == 0) {
		auto q{offset.begin()};
		for (auto& i : line) {
			draw_line(font, i, x + *q++, y);
			y += line_height;
		}
	}
	else {
		for (auto& i : line) {
			draw_line_ralign(font, i, x, y);
			y += line_height;
		}
	}

	return 0;
}

gm::real gm_set_utf16_base64(gm::real enable) {
	setting.utf16_base64 = enable;
	return 0;
}

gm::real gm_set_font(gm::real font) {
	setting.font = (std::size_t)font;
	return 0;
}

gm::real gm_set_color(gm::real color_top, gm::real color_bottom) {
	setting.color_top = (std::uint32_t)color_top;
	setting.color_bottom = (std::uint32_t)color_bottom;
	return 0;
}

gm::real gm_set_alpha(gm::real alpha) {
	setting.alpha = std::clamp(alpha, 0., 1.);
	return 0;
}

gm::real gm_set_align(gm::real halign, gm::real valign) {
	setting.halign = halign == 0 ? 0 : halign < 0 ? -1 : 1;
	setting.valign = valign == 0 ? 0 : valign < 0 ? -1 : 1;
	return 0;
}

gm::real gm_set_max_line_width(gm::real max_width) {
	setting.max_line_width = std::abs(max_width);
	return 0;
}

gm::real gm_set_letter_spacing(gm::real spacing) {
	setting.letter_spacing = spacing;
	return 0;
}

gm::real gm_set_word_spacing(gm::real spacing) {
	setting.word_spacing = spacing;
	return 0;
}

gm::real gm_set_line_height(gm::real multiplier) {
	setting.line_height = std::abs(multiplier);
	return 0;
}

gm::real gm_set_offset(gm::real x, gm::real y) {
	setting.offset_x = x;
	setting.offset_y = y;
	return 0;
}

gm::real gm_set_scale(gm::real x, gm::real y) {
	setting.scale_x = std::abs(x);
	setting.scale_y = std::abs(y);
	return 0;
}