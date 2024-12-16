#pragma once

#include "api.h"

#include <fstream>
#include <unordered_map>

namespace gm {

    struct glyph_data {
        uint16_t x, y;
        uint16_t width;
        int16_t left;
    };

    class font {
        static inline uint32_t _counter{};

        uint32_t _id;
        uint32_t _sprite_id;
        uint16_t _size;
        uint16_t _height;
        std::unordered_map<wchar_t, glyph_data> _glyph;

    public:
        font(std::string_view sprite_path, std::string_view glyph_path) {
            std::ifstream file{ glyph_path.data(), std::ios::binary };
            if (!file.is_open()) {
                throw std::runtime_error{ "Cannot open files." };
            }

            char magic[4];
            file.read(magic, sizeof(magic));
            if (strncmp(magic, "GLY", sizeof(magic)) != 0) {
                throw std::runtime_error{ "Incorrect file format." };
            }

            _id = _counter++;
            _sprite_id = sprite_add(sprite_path.data(), 1, false, false, 0, 0);
            file.read((char*)&_size, sizeof(_size));
            file.read((char*)&_height, sizeof(_height));

            while (file) {
                wchar_t ch;
                file.read((char*)&ch, sizeof(ch));
                file.read((char*)&_glyph[ch], sizeof(_glyph[ch]));
            }
        }

        ~font() noexcept {
            sprite_delete(_sprite_id);
        }

        uint32_t& id() noexcept {
            return _id;
        }

        uint32_t& sprite_id() noexcept {
            return _sprite_id;
        }

        uint16_t& size() noexcept {
            return _size;
        }

        uint16_t& height() noexcept {
            return _height;
        }

        auto& glyph() noexcept {
            return _glyph;
        }
    };

    class font_system : std::vector<font> {
        using base = std::vector<font>;

    public:
        using base::size;
        using base::operator[];

        bool contains(uint32_t font_id) {
            return font_id < size() && (*this)[font_id].size() != 0;
        }

        bool add(std::string_view sprite_path, std::string_view glyph_path) {
            try {
                emplace_back(sprite_path, glyph_path);
                return true;
            }
            catch (...) {
                return false;
            }
        }

        bool remove(uint32_t font_id) {
            if (!contains(font_id)) {
                return false;
            }
            font& font{ (*this)[font_id] };
            sprite_delete(font.sprite_id());
            font.size() = 0;
            font.glyph().clear();
            return true;
        }

        void clear() {
            for (auto& i : *this) {
                sprite_delete(i.sprite_id());
            }
            base::clear();
        }
    };

}