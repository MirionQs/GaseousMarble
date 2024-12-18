#pragma once

#include "font.h"

#include <ranges>

namespace gm {

    struct draw_setting {
        font* font;
        uint32_t color_top, color_bottom;
        double alpha;
        int halign, valign;
        double word_spacing, letter_spacing;
        double max_line_width;
        double line_height;
        double offset_x, offset_y;
        double scale_x, scale_y;
    };

    class draw {
        draw_setting _setting;

        std::wstring _filter(std::wstring_view text) const noexcept {
            std::wstring filtered;
            auto& glyph_map{ _setting.font->glyph() };
            for (auto& ch : text) {
                if (iswblank(ch)) {
                    filtered.push_back(' ');
                }
                else if (ch == '\n' || !iswcntrl(ch) && glyph_map.find(ch) != glyph_map.end()) {
                    filtered.push_back(ch);
                }
            }
            return filtered;
        }

        void _glyph(double x, double y, const glyph_data& glyph) const noexcept {
            draw_sprite_general(
                _setting.font->sprite_id(),
                0,
                glyph.x,
                glyph.y,
                glyph.width,
                _setting.font->height(),
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

        void _line(double x, double y, std::wstring_view line) const noexcept {
            auto& glyph_map{ _setting.font->glyph() };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            for (auto& ch : line) {
                auto& glyph{ glyph_map.at(ch) };
                _glyph(x, y, glyph);
                x += (glyph.left + glyph.width) * _setting.scale_x + letter_spacing;
                if (ch == ' ') {
                    x += word_spacing;
                }
            }
        }

        void _line_backward(double x, double y, std::wstring_view line) const noexcept {
            auto& glyph_map{ _setting.font->glyph() };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            for (auto& ch : line | std::views::reverse) {
                auto& glyph{ glyph_map.at(ch) };
                x -= (glyph.left + glyph.width) * _setting.scale_x;
                _glyph(x, y, glyph);
                x -= letter_spacing;
                if (ch == ' ') {
                    x -= word_spacing;
                }
            }
        }

    public:
        draw() noexcept {
            _setting.font = nullptr;
            _setting.color_top = _setting.color_bottom = 0xffffff;
            _setting.alpha = 1;
            _setting.halign = _setting.valign = -1;
            _setting.word_spacing = _setting.letter_spacing = 0;
            _setting.max_line_width = 0;
            _setting.line_height = 1;
            _setting.offset_x = _setting.offset_y = 0;
            _setting.scale_x = _setting.scale_y = 1;
        }

        draw_setting& setting() noexcept {
            return _setting;
        }

        double width(std::wstring_view text) const noexcept {

        }

        double height(std::wstring_view text) const noexcept {

        }

        bool text(double x, double y, std::wstring_view text) const noexcept {
            if (_setting.font == nullptr) {
                return false;
            }

            std::wstring filtered{ _filter(text) };
            double max_line_width{ _setting.max_line_width * _setting.scale_x };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };
            double line_height{ _setting.line_height * _setting.scale_y * _setting.font->size() };
            double offset_x{ _setting.offset_x * _setting.scale_x };
            double offset_y{ _setting.offset_y * _setting.scale_y };

            std::vector<std::wstring_view> line;
            std::vector<double> offset;
            if (_setting.halign != 0 && max_line_width == 0) {
                for (auto&& i : filtered | std::views::split('\n')) {
                    line.emplace_back(i);
                }
            }
            else {
                auto& glyph_map{ _setting.font->glyph() };
                double line_width{};
                auto begin{ filtered.begin() }, end{ filtered.end() };
                for (auto i{ begin }; i != end; ++i) {
                    if (*i == '\n') {
                        offset.push_back((letter_spacing - line_width) / 2);
                        line.emplace_back(begin, i);
                        line_width = 0;
                        begin = i + 1;
                    }
                    else {
                        auto& glyph{ glyph_map.at(*i) };
                        double char_width{ (glyph.left + glyph.width) * _setting.scale_x + letter_spacing };
                        if (*i == ' ') {
                            char_width += word_spacing;
                        }
                        if (max_line_width == 0 || line_width + char_width <= max_line_width) {
                            line_width += char_width;
                        }
                        else {
                            offset.push_back((letter_spacing - line_width) / 2);
                            line.emplace_back(begin, i);
                            line_width = char_width;
                            begin = i;
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

            return text.size() == filtered.size();
        }
    };

}