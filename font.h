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
    public:
        uint32_t sprite_id;
        uint16_t size;
        uint16_t height;
        std::unordered_map<wchar_t, glyph_data> glyph;

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

            sprite_id = sprite_add(sprite_path.data(), 1, false, false, 0, 0);
            file.read((char*)&size, sizeof(size));
            file.read((char*)&height, sizeof(height));

            while (file) {
                wchar_t ch;
                file.read((char*)&ch, sizeof(ch));
                file.read((char*)&glyph[ch], sizeof(glyph[ch]));
            }
        }

        ~font() noexcept {
            sprite_delete(sprite_id);
        }
    };

    class font_system : std::vector<font> {
        using base = std::vector<font>;

    public:
        using base::size;
        using base::operator[];

        bool contains(uint32_t font_id) const {
            return font_id < size() && (*this)[font_id].size != 0;
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

}