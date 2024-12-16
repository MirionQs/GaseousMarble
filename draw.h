#pragma once

#include "font.h"

#include <ranges>

namespace gm {

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

    class draw {
        font_system _font;
        draw_setting _setting;

        void _char(double x, double y, wchar_t ch) {
            font& font{ _font[_setting.font_id] };
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

        void _line(double x, double y, std::wstring_view line) {
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };

            font& font{ _font[_setting.font_id] };
            for (wchar_t ch : line) {
                glyph_data& glyph{ font.glyph[ch] };
                _char(x, y, ch);
                x += (glyph.left + glyph.width) * _setting.scale_x + letter_spacing;
                if (ch == ' ') {
                    x += word_spacing;
                }
            }
        }

        void _line_backward(double x, double y, std::wstring_view line) {
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };

            font& font{ _font[_setting.font_id] };
            for (wchar_t ch : line | std::views::reverse) {
                glyph_data& glyph{ font.glyph[ch] };
                x -= (glyph.left + glyph.width) * _setting.scale_x;
                _char(x, y, ch);
                x -= letter_spacing;
                if (ch == ' ') {
                    x -= word_spacing;
                }
            }
        }

    public:
        font_system& font_list() {
            return _font;
        }

        draw_setting& setting() {
            return _setting;
        }

        bool text(double x, double y, std::wstring_view text) {
            if (!_font.contains(_setting.font_id)) {
                return false;
            }

            font& font{ _font[_setting.font_id] };

            double max_line_width{ _setting.max_line_width * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double line_height{ _setting.line_height * _setting.scale_y * font.size };
            double offset_x{ _setting.offset_x * _setting.scale_x };
            double offset_y{ _setting.offset_y * _setting.scale_y };

            std::vector<std::wstring_view> line;
            std::vector<double> offset;
            if (_setting.halign != 0 && max_line_width == 0) {
                for (auto&& i : text | std::views::split('\n')) {
                    line.emplace_back(i);
                }
            }
            else {
                double line_width{ 0 };
                auto begin{ text.begin() }, end{ text.end() };
                for (auto p{ begin }; p != end; ++p) {
                    if (*p == '\n') {
                        offset.push_back((letter_spacing - line_width) / 2);
                        line.emplace_back(begin, p);
                        line_width = 0;
                        begin = p + 1;
                    }
                    else {
                        glyph_data& glyph{ font.glyph[*p] };
                        double char_width{ (glyph.left + glyph.width) * _setting.scale_x + letter_spacing };
                        if (*p == ' ') {
                            char_width += word_spacing;
                        }
                        if (max_line_width == 0 || line_width + char_width <= max_line_width) {
                            line_width += char_width;
                        }
                        else {
                            offset.push_back((letter_spacing - line_width) / 2);
                            line.emplace_back(begin, p);
                            line_width = char_width;
                            begin = p;
                        }
                    }
                }
                offset.push_back((letter_spacing - line_width) / 2);
                line.emplace_back(begin, end);
            }

            x += offset_x;
            y += offset_y;
            if (_setting.valign >= 0) {
                double text_height{ line_height * line.size() };
                y -= _setting.valign == 0 ? text_height / 2 : text_height;
            }

            if (_setting.halign < 0) {
                for (auto& i : line) {
                    _line(x, y, i);
                    y += line_height;
                }
            }
            else if (_setting.halign == 0) {
                auto q{ offset.begin() };
                for (auto& i : line) {
                    _line(x + *q++, y, i);
                    y += line_height;
                }
            }
            else {
                for (auto& i : line) {
                    _line_backward(x, y, i);
                    y += line_height;
                }
            }

            return true;
        }
    };
}