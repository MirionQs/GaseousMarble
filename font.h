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
        font() noexcept :
            _id{},
            _sprite_id{},
            _size{},
            _height{} {}

        font(font&& other) noexcept {
            *this = std::move(other);
        }

        font(std::string_view sprite_path, std::string_view glyph_path) :
            _id{ ++_counter } {

            std::ifstream file{ glyph_path.data(), std::ios::binary };
            if (!file.is_open()) {
                return;
            }

            char magic[4];
            file.read(magic, sizeof(magic));
            if (strncmp(magic, "GLY", sizeof(magic)) != 0) {
                return;
            }

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
            if (!empty()) {
                // The code below will throw an exception when quitting the game 
                // because the GameMaker functions have become invalid. But it 
                // doesn't matter.
                sprite_delete(_sprite_id);
            }
        }

        font& operator=(font&& other) noexcept {
            _id = std::exchange(other._id, 0);
            _sprite_id = other._sprite_id;
            _size = other._size;
            _height = other._height;
            _glyph = std::move(other._glyph);
            return *this;
        }

        bool empty() const noexcept {
            return _id == 0;
        }

        uint32_t id() const noexcept {
            return _id;
        }

        uint32_t sprite_id() const noexcept {
            return _sprite_id;
        }

        uint16_t size() const noexcept {
            return _size;
        }

        uint16_t height() const noexcept {
            return _height;
        }

        const auto& glyph() const noexcept {
            return _glyph;
        }
    };

    class font_system {
        std::unordered_map<uint32_t, font> _font;

    public:
        font& operator[](uint32_t font_id) {
            return _font.at(font_id);
        }

        bool contains(uint32_t font_id) {
            return _font.contains(font_id);
        }

        uint32_t add(std::string_view sprite_path, std::string_view glyph_path) {
            font f{ sprite_path, glyph_path };
            if (f.empty()) {
                return 0;
            }
            uint32_t font_id{ f.id() };
            _font.emplace(font_id, std::move(f));
            return font_id;
        }

        bool remove(uint32_t font_id) {
            _font.erase(font_id);
            return true;
        }

        void clear() {
            _font.clear();
        }
    };

}