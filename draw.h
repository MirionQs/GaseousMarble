#pragma once

#include "font.h"

#include <cwctype>

namespace gm {

    struct DrawSetting {
        gm::Font* font;
        gm::u32 color_top, color_bottom;
        gm::f64 alpha;
        gm::i8 halign, valign;
        gm::f64 word_spacing, letter_spacing;
        gm::f64 max_line_width;
        gm::f64 line_height;
        gm::f64 offset_x, offset_y;
        gm::f64 scale_x, scale_y;
    };

    class Draw {
        gm::DrawSetting _setting;

        std::wstring _filter(std::wstring_view text) const noexcept {
            std::wstring filtered;
            auto& glyph_map{ _setting.font->glyph() };

            for (auto& ch : text) {
                if (std::iswblank(ch)) {
                    filtered.push_back(' ');
                }
                else if (ch == '\n' || !std::iswcntrl(ch) && glyph_map.find(ch) != glyph_map.end()) {
                    filtered.push_back(ch);
                }
            }
            return filtered;
        }

        auto _split(std::wstring_view text) const noexcept {
            std::vector<std::pair<std::wstring, gm::f64>> line;
            auto& glyph_map{ _setting.font->glyph() };
            gm::f64 max_line_width{ _setting.max_line_width * _setting.scale_x };
            gm::f64 word_spacing{ _setting.word_spacing * _setting.scale_x };
            gm::f64 letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            auto begin{ text.begin() }, end{ text.end() }, i{ begin };
            gm::f64 line_width{}, last_spacing{};
            while (i != end) {
                if (*i == '\n') {
                    line.emplace_back(std::wstring{ begin, i }, line_width - last_spacing);
                    line_width = last_spacing = 0;
                    begin = ++i;
                    continue;
                }

                gm::f64 spacing{ letter_spacing };
                if (*i == ' ') {
                    spacing += word_spacing;
                }
                auto& glyph{ glyph_map.at(*i) };
                gm::f64 char_width{ (glyph.left + glyph.width) * _setting.scale_x + spacing };

                if (max_line_width == 0 || line_width + char_width - spacing <= max_line_width) {
                    line_width += char_width;
                }
                else {
                    line.emplace_back(std::wstring{ begin, i }, line_width - last_spacing);
                    line_width = char_width;
                    begin = i;
                }
                last_spacing = spacing;
                ++i;
            }
            line.emplace_back(std::wstring{ begin, end }, line_width - last_spacing);
            return line;
        }

        void _glyph(gm::f64 x, gm::f64 y, const gm::GlyphData& glyph) const noexcept {
            gm::draw_sprite_general(
                _setting.font->sprite().id(),
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

        void _line(gm::f64 x, gm::f64 y, std::wstring_view text) const noexcept {
            auto& glyph_map{ _setting.font->glyph() };
            gm::f64 word_spacing{ _setting.word_spacing * _setting.scale_x };
            gm::f64 letter_spacing{ _setting.letter_spacing * _setting.scale_x };

            for (auto& ch : text) {
                auto& glyph{ glyph_map.at(ch) };
                _glyph(x, y, glyph);
                x += (glyph.left + glyph.width) * _setting.scale_x + letter_spacing;
                if (ch == ' ') {
                    x += word_spacing;
                }
            }
        }

    public:
        Draw() noexcept {
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

        gm::DrawSetting& setting() noexcept {
            return _setting;
        }

        gm::f64 width(std::wstring_view text) const noexcept {
            auto line{ _split(_filter(text)) };
            gm::f64 max_width{};
            for (auto& [text, width] : line) {
                max_width = std::max(max_width, width);
            }
            return max_width;
        }

        gm::f64 height(std::wstring_view text) const noexcept {
            auto line{ _split(_filter(text)) };
            gm::f64 line_height{ _setting.line_height * _setting.scale_y * _setting.font->size() };
            return line_height * line.size();
        }

        bool text(gm::f64 x, gm::f64 y, std::wstring_view text) const noexcept {
            if (_setting.font == nullptr) {
                return false;
            }

            std::wstring filtered{ _filter(text) };
            auto line{ _split(filtered) };
            gm::f64 line_height{ _setting.line_height * _setting.scale_y * _setting.font->size() };
            gm::f64 offset_x{ _setting.offset_x * _setting.scale_x };
            gm::f64 offset_y{ _setting.offset_y * _setting.scale_y };

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