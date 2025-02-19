module;

#include <assert.h>
#include <d3dx8tex.h>

export module gm.engine;

import std;
import gm.core;

using namespace gm::core;

// fundamental types of GML
namespace gm::engine {

    export using Real = f64;

    struct StringHeader {
        u16 code_page;
        u16 char_size;
        u32 ref_count;
        u32 size;
    };

    export class String {
        char* _data;

        auto _header() noexcept {
            return reinterpret_cast<StringHeader*>(_data - sizeof(StringHeader));
        }

        auto _header() const noexcept {
            return reinterpret_cast<const StringHeader*>(_data - sizeof(StringHeader));
        }

    public:
        String() noexcept {
            static String empty_string{ "" };
            _data = empty_string._data;
            ++_header()->ref_count;
        }

        String(std::string_view string) noexcept :
            _data{ new char[sizeof(StringHeader) + string.size() + 1] + sizeof(StringHeader) } {

            new(_header()) StringHeader{ 65001, 1, 1, string.size() };
            std::memcpy(_data, string.data(), string.size() + 1);
        }

        String(const String& other) noexcept :
            _data{ other._data } {

            ++_header()->ref_count;
        }

        ~String() noexcept {
            if (--_header()->ref_count == 0) {
                delete[](_data - sizeof(StringHeader));
            }
        }

        String& operator=(const String& other) noexcept {
            if (this == &other) {
                return *this;
            }
            String temp{ other };
            std::swap(_data, temp._data);
            return *this;
        }

        operator std::string_view() const noexcept {
            return { _data, _header()->size };
        }

        u32 size() const noexcept {
            return _header()->size;
        }

        u32 ref_count() const noexcept {
            return _header()->ref_count;
        }

        const char* data() const noexcept {
            return _data;
        }
    };

    // used for external strings as their lifetime cannot be managed
    export class StringView {
        const char* _data;

        auto _header() const noexcept {
            return reinterpret_cast<const StringHeader*>(_data - sizeof(StringHeader));
        }

    public:
        StringView(String string = {}) noexcept :
            _data{ string.data() } {}

        operator std::string_view() const noexcept {
            return { _data, _header()->size };
        }

        u32 size() const noexcept {
            return _header()->size;
        }

        u32 ref_count() const noexcept {
            return _header()->ref_count;
        }

        const char* data() const noexcept {
            return _data;
        }
    };

}

// interface of GameMaker function resources
namespace gm::engine {

    enum class ValueType {
        real,
        string
    };

    class Value {
        ValueType _type{ ValueType::real };
        Real _real{};
        String _string{};

    public:
        Value() noexcept = default;

        Value(Real real) noexcept :
            _real{ real } {}

        Value(String string) noexcept :
            _type{ ValueType::string },
            _string{ string } {}

        operator Real() const noexcept {
            assert(_type == ValueType::real);
            return _real;
        }

        operator String() const noexcept {
            assert(_type == ValueType::string);
            return _string;
        }

        ValueType type() const noexcept {
            return _type;
        }
    };

    export class Function {
        u8 _name_length;
        char _name[67];
        void* _address;
        i32 _arg_count;
        bool _require_pro;

    public:
        Function() = delete;

        std::string_view name() const noexcept {
            return { _name, _name_length };
        }

        // -1 indicates variable arguments
        i32 arg_count() const noexcept {
            return _arg_count;
        }

        void* address() const noexcept {
            return _address;
        }

        template<class R, class... Args>
        R call(Args... args) const noexcept {
            Value args_wrapped[]{ args... }, returned;
            Value* argv{ args_wrapped };
            constexpr u32 argc{ sizeof...(args) };
            Value* pret{ &returned };
            void* pfn{ _address };

            // This assertion will fail when quitting the game because the
            // resources have already been released by GameMaker.
            assert(_arg_count == -1 || _arg_count == argc);

            __asm {
                push argv;
                push argc;
                push pret;
                call pfn;
            }

            return static_cast<R>(returned);
        }
    };

    struct FunctionResource {
        Function* functions;
        u32 count;
    };

    export enum class FunctionId {
#include "inc/FunctionId.inc"
    };

    class IFunction {
        FunctionResource* _resource;

    public:
        IFunction() noexcept :
            _resource{ reinterpret_cast<FunctionResource*>(0x00686b1c) } {};

        Function& operator[](FunctionId id) const noexcept {
            return _resource->functions[static_cast<u32>(id)];
        }

        u32 count() const noexcept {
            return _resource->count;
        }
    };

    export IFunction function;

}

// interface of GameMaker texture resources
namespace gm::engine {

    export class Texture {
        IDirect3DTexture8* _data;
        Size _image_size;
        Size _texture_size;
        bool _is_valid;

    public:
        Texture() = delete;

        auto&& image_size(this auto& self) noexcept {
            return std::forward_like<decltype(self)>(self._image_size);
        }

        auto&& texture_size(this auto& self) noexcept {
            return std::forward_like<decltype(self)>(self._texture_size);
        }

        auto&& data(this auto& self) noexcept {
            return std::forward_like<decltype(self)>(self._data);
        }
    };

    class ITexture {
        Texture** _textures;
        u32* _count;

    public:
        ITexture() noexcept :
            _textures{ reinterpret_cast<Texture**>(0x0085b3c4) },
            _count{ reinterpret_cast<u32*>(0x006886f0) } {};

        Texture& operator[](u32 id) const noexcept {
            assert(id < *_count);
            return (*_textures)[id];
        }

        u32 count() const noexcept {
            return *_count;
        }
    };

    export ITexture texture;

}

// interface of GameMaker sprite resources
namespace gm::engine {

    export class Bitmap {
        void* _rtti;
        Size _size;
        void* _data;

    public:
        Bitmap() = delete;

        auto&& size(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self._size);
        }
    };

    struct SpriteData {
        void* rtti;
        u32 subimage_count;
        Bitmap** bitmaps;
        Point origin;
        BoundingBox bounding_box;
        void* masks;
        bool seperate_masks;
        u32* texture_ids;
    };

    export class Sprite {
        SpriteData* _data;
        std::wstring_view _name;

    public:
        Sprite() = delete;

        Sprite(SpriteData* data, wchar_t* name) noexcept : _data{ data }, _name{ name } {}

        std::wstring_view name() const noexcept {
            return _name;
        }

        u32 subimage_count() const noexcept {
            return _data->subimage_count;
        }

        auto&& origin(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self._data->origin);
        }

        auto&& bounding_box(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self._data->bounding_box);
        }

        auto&& bitmap(this auto&& self, u32 index) noexcept {
            assert(index < self._data->subimage_count);
            return std::forward_like<decltype(self)>(*self._data->bitmaps[index]);
        }

        auto&& texture(this auto&& self, u32 index) noexcept {
            assert(index < self._data->subimage_count);
            return std::forward_like<decltype(self)>(gm::engine::texture[self._data->texture_ids[index]]);
        }

        void set_texture(u32 index, u32 id) noexcept {
            assert(index < _data->subimage_count && id < gm::engine::texture.count());
            _data->texture_ids[index] = id;
        }
    };

    class ISprite {
        struct SpriteResource {
            SpriteData** sprites;
            wchar_t** names;
            u32 count;
        };

        SpriteResource* _resource;

    public:
        ISprite() noexcept :
            _resource{ reinterpret_cast<SpriteResource*>(0x00686ac8) } {};

        Sprite operator[](u32 id) const noexcept {
            assert(id < _resource->count);
            return { _resource->sprites[id], _resource->names[id] };
        }

        u32 count() const noexcept {
            return _resource->count;
        }

        u32 find(std::wstring_view name) const noexcept {
            for (u32 id{}; id != _resource->count; ++id) {
                if (std::wcscmp(name.data(), _resource->names[id]) == 0) {
                    return id;
                }
            }
            return -1;
        }
    };

    export ISprite sprite;

}