#pragma once

#include "sprite.h"

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
        uint16_t _size;
        uint16_t _height;
        std::unique_ptr<sprite_handle, sprite_deleter> _sprite;
        std::unordered_map<wchar_t, glyph_data> _glyph;

    public:
        font() noexcept :
            _id{},
            _size{},
            _height{},
            _sprite{} {}

        font(std::string_view sprite_path, std::string_view glyph_path) noexcept :
            _id{ ++_counter },
            _size{},
            _height{},
            _sprite{ sprite_add(sprite_path.data(), 1, false, false, 0, 0) } {

            std::ifstream file{ glyph_path.data(), std::ios::binary };
            if (!file.is_open()) {
                return;
            }

            char magic[4];
            file.read(magic, sizeof(magic));
            if (strncmp(magic, "GLY", sizeof(magic)) != 0) {
                return;
            }

            file.read((char*)&_size, sizeof(_size));
            file.read((char*)&_height, sizeof(_height));

            while (file) {
                wchar_t ch;
                file.read((char*)&ch, sizeof(ch));
                file.read((char*)&_glyph[ch], sizeof(_glyph[ch]));
            }
        }

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(const font& other) const noexcept {
            return _id == other.id();
        }

        uint32_t id() const noexcept {
            return _id;
        }

        uint16_t size() const noexcept {
            assert(_id != 0);
            return _size;
        }

        uint16_t height() const noexcept {
            assert(_id != 0);
            return _height;
        }

        sprite_handle sprite() const noexcept {
            assert(_id != 0);
            return _sprite.get();
        }

        const auto& glyph() const noexcept {
            assert(_id != 0);
            return _glyph;
        }
    };

}