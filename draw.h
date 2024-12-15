#pragma once

#include "api.h"

#include <fstream>
#include <ranges>
#include <unordered_map>

namespace gm {

	struct glyph_data {
		uint16_t x, y;
		uint16_t width;
		int16_t left;
	};

	struct font_data {
		size_t sprite_id;
		uint16_t size;
		uint16_t glyph_height;
		std::unordered_map<wchar_t, glyph_data> glyph;
	};

	class font_system : std::vector<font_data> {
		using base = std::vector<font_data>;

	public:
		using base::size;
		using base::operator[];

		bool contains(size_t font_id) const {
			return font_id < size() && (*this)[font_id].size != 0;
		}

		bool add(std::string_view sprite_path, std::string_view glyph_path) {
			std::ifstream file{ glyph_path.data(), std::ios::binary };

			char magic[4];
			file.read(magic, 4);
			magic[3] = 0;
			if (strcmp(magic, "GLY") != 0) {
				return false;
			}

			font_data font;
			font.sprite_id = sprite_add(sprite_path.data(), 1, false, false, 0, 0);
			file.read((char*)&font.size, sizeof(uint16_t));
			file.read((char*)&font.glyph_height, sizeof(uint16_t));
			if (font.size == 0 || font.glyph_height == 0) {
				return false;
			}

			while (file) {
				wchar_t ch;
				file.read((char*)&ch, sizeof(wchar_t));
				file.read((char*)&font.glyph[ch], sizeof(glyph_data));
			}

			push_back(std::move(font));
			return true;
		}

		bool remove(size_t font_id) {
			if (!contains(font_id)) {
				return false;
			}
			font_data& font{ (*this)[font_id] };
			sprite_delete(font.sprite_id);
			font.size = 0;
			font.glyph.clear();
			return true;
		}

		void clear() {
			for (auto& i : *this) {
				sprite_delete(i.sprite_id);
			}
			base::clear();
		}
	};

	struct draw_setting {
		size_t font_id{ 0 };
		uint32_t color_top{ 0xffffff }, color_bottom{ 0xffffff };
		double alpha{ 1 };
		int halign{ -1 }, valign{ -1 };
		double max_line_width{ 0 };
		double letter_spacing{ 0 };
		double word_spacing{ 0 };
		double line_height{ 1 };
		double offset_x{ 0 }, offset_y{ 0 };
		double scale_x{ 1 }, scale_y{ 1 };
	};

	class draw_system {
		font_system _font;
		draw_setting _setting;

		void _drawChar(double x, double y, wchar_t ch) {
			font_data& font{ _font[_setting.font_id] };
			glyph_data& glyph{ font.glyph[ch] };
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

		void _drawLine(double x, double y, std::wstring_view line) {
			double scaledLetterSpacing{ _setting.letter_spacing * _setting.scale_x };
			double scaledWordSpacing{ _setting.word_spacing * _setting.scale_x };

			font_data& font{ _font[_setting.font_id] };
			for (wchar_t ch : line) {
				glyph_data& glyph{ font.glyph[ch] };
				_drawChar(x, y, ch);
				x += (glyph.left + glyph.width) * _setting.scale_x + scaledLetterSpacing;
				if (ch == ' ') {
					x += scaledWordSpacing;
				}
			}
		}

		void _drawLineBackward(double x, double y, std::wstring_view line) {
			double scaledLetterSpacing{ _setting.letter_spacing * _setting.scale_x };
			double scaledWordSpacing{ _setting.word_spacing * _setting.scale_x };

			font_data& font{ _font[_setting.font_id] };
			for (wchar_t ch : line | std::views::reverse) {
				glyph_data& glyph{ font.glyph[ch] };
				x -= (glyph.left + glyph.width) * _setting.scale_x;
				_drawChar(x, y, ch);
				x -= scaledLetterSpacing;
				if (ch == ' ') {
					x -= scaledWordSpacing;
				}
			}
		}

	public:
		font_system& font() {
			return _font;
		}

		draw_setting& setting() {
			return _setting;
		}

		bool draw(double x, double y, std::wstring_view text) {
			if (!_font.contains(_setting.font_id)) {
				return false;
			}

			font_data& font{ _font[_setting.font_id] };

			double scaledMaxLineWidth{ _setting.max_line_width * _setting.scale_x };
			double scaledLetterSpacing{ _setting.letter_spacing * _setting.scale_x };
			double scaledWordSpacing{ _setting.word_spacing * _setting.scale_x };
			double scaledLineHeight{ _setting.line_height * _setting.scale_y * font.size };
			double scaledOffsetX{ _setting.offset_x * _setting.scale_x };
			double scaledOffsetY{ _setting.offset_y * _setting.scale_y };

			std::vector<std::wstring_view> line;
			std::vector<double> offset;
			if (_setting.halign != 0 && scaledMaxLineWidth == 0) {
				for (auto&& i : text | std::views::split('\n')) {
					line.emplace_back(i);
				}
			}
			else {
				double lineWidth{ 0 };
				auto begin{ text.begin() }, end{ text.end() };
				for (auto p{ begin }; p != end; ++p) {
					if (*p == '\n') {
						offset.push_back((scaledLetterSpacing - lineWidth) / 2);
						line.emplace_back(begin, p);
						lineWidth = 0;
						begin = p + 1;
					}
					else {
						glyph_data& glyph{ font.glyph[*p] };
						double charWidth{ (glyph.left + glyph.width) * _setting.scale_x + scaledLetterSpacing };
						if (*p == ' ') {
							charWidth += scaledWordSpacing;
						}
						if (scaledMaxLineWidth == 0 || lineWidth + charWidth <= scaledMaxLineWidth) {
							lineWidth += charWidth;
						}
						else {
							offset.push_back((scaledLetterSpacing - lineWidth) / 2);
							line.emplace_back(begin, p);
							lineWidth = charWidth;
							begin = p;
						}
					}
				}
				offset.push_back((scaledLetterSpacing - lineWidth) / 2);
				line.emplace_back(begin, end);
			}

			x += scaledOffsetX;
			y += scaledOffsetY;
			if (_setting.valign >= 0) {
				double textHeight{ scaledLineHeight * line.size() };
				y -= _setting.valign == 0 ? textHeight / 2 : textHeight;
			}

			if (_setting.halign < 0) {
				for (auto& i : line) {
					_drawLine(x, y, i);
					y += scaledLineHeight;
				}
			}
			else if (_setting.halign == 0) {
				auto q{ offset.begin() };
				for (auto& i : line) {
					_drawLine(x + *q++, y, i);
					y += scaledLineHeight;
				}
			}
			else {
				for (auto& i : line) {
					_drawLineBackward(x, y, i);
					y += scaledLineHeight;
				}
			}

			return true;
		}
	};
}