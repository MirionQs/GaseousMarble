#pragma once

#include "utils.h"

#include <fstream>
#include <ranges>
#include <unordered_map>

namespace gm {

	struct gm_api {
		function<void*, string> get_function_pointer;
		function<std::size_t, string, real, real, real, real, real> sprite_add;
		function<void, real> sprite_delete;
		function<void, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real> draw_sprite_general;

		gm_api(void* ptr = nullptr) {
			if (ptr == nullptr) {
				return;
			}
			get_function_pointer = ptr;
			sprite_add = get_function_pointer("sprite_add");
			sprite_delete = get_function_pointer("sprite_delete");
			draw_sprite_general = get_function_pointer("draw_sprite_general");
		}
	};

	struct glyph_data {
		std::uint16_t x, y;
		std::uint8_t width;
		std::int8_t left;
	};

	struct font_data {
		std::size_t sprite_id;
		std::uint8_t size;
		std::uint8_t glyph_height;
		std::unordered_map<wchar_t, glyph_data> glyph;

		operator bool() {
			return size != 0;
		}
	};

	struct draw_setting {
		std::size_t font_id{0};
		std::uint32_t color_top{0xffffff}, color_bottom{0xffffff};
		double alpha{1};
		int halign{-1}, valign{-1};
		double max_line_width{0};
		double letter_spacing{0};
		double word_spacing{0};
		double line_height{1};
		double offset_x{0}, offset_y{0};
		double scale_x{1}, scale_y{1};
	};

	class draw_system {
		gm_api _api;
		draw_setting _setting;
		std::vector<font_data> _font_set;

	public:
		draw_system(const gm_api& api = nullptr) {
			_api = api;
		}

		draw_setting& setting() {
			return _setting;
		}

		std::size_t add_font(std::string_view sprite_path, std::string_view glyph_path) {
			std::ifstream file{glyph_path.data(), std::ios::binary};
			char header[6];
			file.read(header, 6);
			if (std::strcmp(header, "GLY") != 0) {
				return -1;
			}

			font_data font;
			font.sprite_id = _api.sprite_add(sprite_path.data(), 1, false, false, 0, 0);
			font.size = (std::uint8_t)header[4];
			font.glyph_height = (std::uint8_t)header[5];

			while (file) {
				char buffer[8];
				file.read(buffer, 8);

				std::uint8_t* view{(std::uint8_t*)buffer};
				font.glyph[view[0] << 8 | view[1]] = {
					(std::uint16_t)(view[2] << 8 | view[3]),
					(std::uint16_t)(view[4] << 8 | view[5]),
					view[6],
					(std::int8_t)view[7]
				};
			}

			_font_set.push_back(std::move(font));
			return _font_set.size() - 1;
		}

		bool set_font(std::size_t font_id) {
			if (font_id >= _font_set.size() || !_font_set[font_id]) {
				return false;
			}
			_setting.font_id = font_id;
			return true;
		}

		void remove_font(std::size_t font_id) {
			if (font_id >= _font_set.size()) {
				return;
			}
			font_data& font{_font_set[_setting.font_id]};
			_api.sprite_delete(font.sprite_id);
			font.size = 0;
			font.glyph.clear();
		}

		void draw_character(double x, double y, wchar_t ch) {
			font_data& font{_font_set[_setting.font_id]};
			glyph_data& glyph{font.glyph[ch]};
			_api.draw_sprite_general(
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

		void draw_line(double x, double y, std::wstring_view line) {
			double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
			double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};
			font_data& font{_font_set[_setting.font_id]};
			for (wchar_t ch : line) {
				glyph_data& glyph{font.glyph[ch]};
				draw_character(x, y, ch);
				x += (glyph.left + glyph.width) * _setting.scale_x + scaled_letter_spacing;
				if (ch == ' ') {
					x += scaled_word_spacing;
				}
			}
		}

		void draw_line_ralign(double x, double y, std::wstring_view line) {
			double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
			double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};
			font_data& font{_font_set[_setting.font_id]};
			for (wchar_t ch : line | std::views::reverse) {
				glyph_data& glyph{font.glyph[ch]};
				x -= (glyph.left + glyph.width) * _setting.scale_x;
				draw_character(x, y, ch);
				x -= scaled_letter_spacing;
				if (ch == ' ') {
					x -= scaled_word_spacing;
				}
			}
		}

		bool draw_text(double x, double y, std::wstring_view text) {
			if (_setting.font_id >= _font_set.size() || !_font_set[_setting.font_id]) {
				return false;
			}

			font_data& font{_font_set[_setting.font_id]};
			double scaled_max_line_width{_setting.max_line_width * _setting.scale_x};
			double scaled_letter_spacing{_setting.letter_spacing * _setting.scale_x};
			double scaled_word_spacing{_setting.word_spacing * _setting.scale_x};
			double scaled_line_height{_setting.line_height * _setting.scale_y * font.size * 4 / 3};
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
					draw_line(x, y, i);
					y += scaled_line_height;
				}
			}
			else if (_setting.halign == 0) {
				auto q{offset.begin()};
				for (auto& i : line) {
					draw_line(x + *q++, y, i);
					y += scaled_line_height;
				}
			}
			else {
				for (auto& i : line) {
					draw_line_ralign(x, y, i);
					y += scaled_line_height;
				}
			}

			return true;
		}
	};
}