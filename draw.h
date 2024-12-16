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