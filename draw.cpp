#include "draw.h"

bool gm::font_system::contains(size_t font_id) {
	return font_id < size() && (*this)[font_id].size != 0;
}

bool gm::font_system::add(std::string_view sprite_path, std::string_view glyph_path) {
	std::ifstream file{glyph_path.data(), std::ios::binary};

	char magic[4];
	file.read(magic, 4);
	if (strcmp(magic, "GLY") != 0) {
		return false;
	}

	font_data font;
	font.sprite_id = sprite_add(sprite_path.data(), 1, false, false, 0, 0);
	file.read((char*)&font.size, 2);
	file.read((char*)&font.glyph_height, 2);
	if (font.size == 0 || font.glyph_height == 0) {
		return false;
	}

	while (file) {
		wchar_t ch;
		glyph_data glyph;
		file.read((char*)&ch, 2);
		file.read((char*)&glyph, 8);
		font.glyph[ch] = std::move(glyph);
	}

	push_back(std::move(font));
	return true;
}

bool gm::font_system::remove(size_t font_id) {
	if (!contains(font_id)) {
		return false;
	}
	font_data& font{(*this)[font_id]};
	sprite_delete(font.sprite_id);
	font.size = 0;
	font.glyph.clear();
	return true;
}

void gm::font_system::clear() {
	for (auto& i : *this) {
		sprite_delete(i.sprite_id);
	}
	base::clear();
}

void gm::draw_system::_drawChar(double x, double y, wchar_t ch) {
	font_data& font{_font[_setting.font_id]};
	glyph_data& glyph{font.glyph[ch]};
	draw_sprite_general(
		font.sprite_id,
		0,
		glyph.x,
		glyph.y,
		glyph.width,
		font.glyph_height,
		x + glyph.left * _setting.scale_x,
		y,
		_setting.scale_x,
		_setting.scale_y,
		0,
		_setting.color_top,
		_setting.color_top,
		_setting.color_bottom,
		_setting.color_bottom,
		_setting.alpha
	);
}

void gm::draw_system::_drawLine(double x, double y, std::wstring_view line) {
	double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
	double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};

	font_data& font{_font[_setting.font_id]};
	for (wchar_t ch : line) {
		glyph_data& glyph{font.glyph[ch]};
		_drawChar(x, y, ch);
		x += (glyph.left + glyph.width) * _setting.scale_x + scaled_letter_spacing;
		if (ch == ' ') {
			x += scaled_word_spacing;
		}
	}
}

void gm::draw_system::_drawLineR(double x, double y, std::wstring_view line) {
	double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
	double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};

	font_data& font{_font[_setting.font_id]};
	for (wchar_t ch : line | std::views::reverse) {
		glyph_data& glyph{font.glyph[ch]};
		x -= (glyph.left + glyph.width) * _setting.scale_x;
		_drawChar(x, y, ch);
		x -= scaled_letter_spacing;
		if (ch == ' ') {
			x -= scaled_word_spacing;
		}
	}
}

gm::font_system& gm::draw_system::font() {
	return _font;
}

gm::draw_setting& gm::draw_system::setting() {
	return _setting;
}

bool gm::draw_system::draw(double x, double y, std::wstring_view text) {
	if (!_font.contains(_setting.font_id)) {
		return false;
	}

	font_data& font{_font[_setting.font_id]};

	double scaled_max_line_width{_setting.max_line_width * _setting.scale_x};
	double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
	double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};
	double scaled_line_height{_setting.line_height * _setting.scale_y * font.size};
	double scaled_offset_x{_setting.offset_x * _setting.scale_x};
	double scaled_offset_y{_setting.offset_y * _setting.scale_y};

	std::vector<std::wstring_view> line;
	std::vector<double> offset;
	if (_setting.halign != 0 && scaled_max_line_width == 0) {
		for (auto&& i : text | std::views::split('\n')) {
			line.emplace_back(i);
		}
	}
	else {
		double line_width{0};
		auto begin{text.begin()}, end{text.end()};
		for (auto p{begin}; p != end; ++p) {
			if (*p == '\n') {
				offset.push_back((scaled_letter_spacing - line_width) / 2);
				line.emplace_back(begin, p);
				line_width = 0;
				begin = p + 1;
			}
			else {
				glyph_data& glyph{font.glyph[*p]};
				double char_width{(glyph.left + glyph.width) * _setting.scale_x + scaled_letter_spacing};
				if (*p == ' ') {
					char_width += scaled_word_spacing;
				}
				if (scaled_max_line_width == 0 || line_width + char_width <= scaled_max_line_width) {
					line_width += char_width;
				}
				else {
					offset.push_back((scaled_letter_spacing - line_width) / 2);
					line.emplace_back(begin, p);
					line_width = char_width;
					begin = p;
				}
			}
		}
		offset.push_back((scaled_letter_spacing - line_width) / 2);
		line.emplace_back(begin, end);
	}

	x += scaled_offset_x;
	y += scaled_offset_y;
	if (_setting.valign >= 0) {
		double text_height{scaled_line_height * line.size()};
		y -= _setting.valign == 0 ? text_height / 2 : text_height;
	}

	if (_setting.halign < 0) {
		for (auto& i : line) {
			_drawLine(x, y, i);
			y += scaled_line_height;
		}
	}
	else if (_setting.halign == 0) {
		auto q{offset.begin()};
		for (auto& i : line) {
			_drawLine(x + *q++, y, i);
			y += scaled_line_height;
		}
	}
	else {
		for (auto& i : line) {
			_drawLineR(x, y, i);
			y += scaled_line_height;
		}
	}

	return true;
}