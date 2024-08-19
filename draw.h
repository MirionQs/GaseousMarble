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

		bool contains(size_t font_id);
		bool add(std::string_view sprite_path, std::string_view glyph_path);
		bool remove(size_t font_id);
		void clear();
	};

	struct draw_setting {
		size_t font_id{0};
		uint32_t color_top{0xffffff}, color_bottom{0xffffff};
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
		font_system _font;
		draw_setting _setting;

		void _drawChar(double x, double y, wchar_t ch);
		void _drawLine(double x, double y, std::wstring_view line);
		void _drawLineR(double x, double y, std::wstring_view line);

	public:
		font_system& font();
		draw_setting& setting();

		bool draw(double x, double y, std::wstring_view text);
	};
}