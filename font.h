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

    struct font {
        size_t sprite_id;
        uint16_t size;
        uint16_t glyph_height;
        std::unordered_map<wchar_t, glyph_data> glyph;
    };

    class font_system : std::vector<font> {
        using base = std::vector<font>;

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

            font font;
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