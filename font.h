#pragma once

#include "sprite.h"

#include <fstream>
#include <unordered_map>

namespace gm {

    struct GlyphData {
        gm::u16 x, y;
        gm::u16 width;
        gm::i16 left;
    };

    class Font {
        static inline gm::u32 _counter;

        gm::u32 _id;
        gm::u16 _size;
        gm::u16 _height;
        std::unique_ptr<gm::SpriteHandle, gm::SpriteDeleter> _sprite;
        std::unordered_map<wchar_t, gm::GlyphData> _glyph;

    public:
        Font() noexcept :
            _id{},
            _size{},
            _height{},
            _sprite{} {}

        Font(std::string_view sprite_path, std::string_view glyph_path) noexcept :
            _id{ ++_counter },
            _size{},
            _height{},
            _sprite{ gm::sprite_add(sprite_path, 1, false, false, 0, 0) } {

            std::ifstream file{ glyph_path.data(), std::ios::binary };
            if (!file.is_open()) {
                return;
            }

            char magic[4];
            file.read(magic, sizeof(magic));
            if (std::strncmp(magic, "GLY", sizeof(magic)) != 0) {
                return;
            }

            file.read(reinterpret_cast<char*>(&_size), sizeof(_size));
            file.read(reinterpret_cast<char*>(&_height), sizeof(_height));

            while (file) {
                wchar_t ch;
                file.read(reinterpret_cast<char*>(&ch), sizeof(ch));
                file.read(reinterpret_cast<char*>(&_glyph[ch]), sizeof(_glyph[ch]));
            }
        }

        operator bool() const noexcept {
            return _id != 0;
        }

        bool operator==(const Font& other) const noexcept {
            return _id == other.id();
        }

        gm::u32 id() const noexcept {
            return _id;
        }

        gm::u16 size() const noexcept {
            assert(_id != 0);
            return _size;
        }

        gm::u16 height() const noexcept {
            assert(_id != 0);
            return _height;
        }

        gm::SpriteHandle sprite() const noexcept {
            assert(_id != 0);
            return _sprite.get();
        }

        const auto& glyph() const noexcept {
            assert(_id != 0);
            return _glyph;
        }
    };

}