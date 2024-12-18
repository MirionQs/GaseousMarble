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

        auto _split(std::wstring_view text) const noexcept {
            std::vector<std::pair<std::wstring, double>> line;
            auto& glyph_map{ _setting.font->glyph() };
            double max_line_width{ _setting.max_line_width * _setting.scale_x };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            auto begin{ text.begin() }, end{ text.end() };
            double line_width{};
            for (auto i{ begin }; i != end; ++i) {
                if (*i == '\n') {
                    line.emplace_back(std::wstring{ begin, i }, line_width - letter_spacing);
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
                        line.emplace_back(std::wstring{ begin, i }, line_width - letter_spacing);
                        line_width = char_width;
                        begin = i;
                    }
                }
            }
            line.emplace_back(std::wstring{ begin, end }, line_width - letter_spacing);
            return line;
        }

        void _char(double x, double y, const glyph_data& glyph) const noexcept {
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

        void _line(double x, double y, std::wstring_view text) const noexcept {
            auto& glyph_map{ _setting.font->glyph() };
            double word_spacing{ _setting.word_spacing * _setting.scale_x };
            double letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            for (auto& ch : text) {
                auto& glyph{ glyph_map.at(ch) };
                _char(x, y, glyph);
                x += (glyph.left + glyph.width) * _setting.scale_x + letter_spacing;
                if (ch == ' ') {
                    x += word_spacing;
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
            auto line{ _split(_filter(text)) };
            double max_width{};
            for (auto& [text, width] : line) {
                max_width = std::max(max_width, width);
            }
            return max_width;
        }

        double height(std::wstring_view text) const noexcept {
            auto line{ _split(_filter(text)) };
            double line_height{ _setting.line_height * _setting.scale_y * _setting.font->size() };
            return line_height * line.size();
        }

        bool text(double x, double y, std::wstring_view text) const noexcept {
            if (_setting.font == nullptr) {
                return false;
            }

            std::wstring filtered{ _filter(text) };
            auto line{ _split(filtered) };
            double line_height{ _setting.line_height * _setting.scale_y * _setting.font->size() };
            double offset_x{ _setting.offset_x * _setting.scale_x };
            double offset_y{ _setting.offset_y * _setting.scale_y };

            x += offset_x;
            y += offset_y;
            if (_setting.valign < 0) {
                // do nothing
            }
            if (_setting.valign == 0) {
                y -= line_height * line.size() / 2;
            }
            else {
                y -= line_height * line.size();
            }

            if (_setting.halign < 0) {
                for (auto& [text, width] : line) {
                    _line(x, y, text);
                    y += line_height;
                }
            }
            else if (_setting.halign == 0) {
                for (auto& [text, width] : line) {
                    _line(x - width / 2, y, text);
                    y += line_height;
                }
            }
            else {
                for (auto& [text, width] : line) {
                    _line(x - width, y, text);
                    y += line_height;
                }
            }

            return text.size() == filtered.size();
        }
    };

}