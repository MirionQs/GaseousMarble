#pragma once

#include <cassert>
#include <utility>

namespace gm {

    using real = double;
    using string = const char*;

    class value {
        bool _is_string;
        real _real;
        string _string;

    public:
        value() noexcept :
            _is_string{},
            _real{},
            _string{} {}

        value(real num) noexcept :
            _is_string{},
            _real{ num },
            _string{} {}

        value(string str) noexcept :
            _is_string{ true },
            _real{} {

            assert(str != nullptr);

            size_t len{ strlen(str) };
            char* pas_str{ new char[len + 13] };
            new(pas_str) uint64_t{};
            new(pas_str + 8) uint32_t{ len };
            memcpy(pas_str + 12, str, len + 1);

            _string = pas_str + 12;
        }

        ~value() noexcept {
            if (_is_string) {
                delete[](_string - 12);
            }
        }

        operator real() const noexcept {
            assert(!_is_string);
            return _real;
        }

        operator string() const noexcept {
            assert(_is_string);
            return _string;
        }

        value(const value&) = delete;

        value& operator=(const value&) = delete;
    };

    template<class R, class... Args>
    class function {
        void* _ptr;

    public:
        function(void* ptr = nullptr) noexcept :
            _ptr{ ptr } {}

        R operator()(Args... args) const noexcept {
            assert(_ptr != nullptr);

            value wrapped[]{ args... }, ret;
            value* argv{ wrapped }, * pret{ &ret };
            constexpr uint32_t argc{ sizeof...(args) };
            void* pfn{ _ptr };
            __asm {
                push argv;
                push argc;
                push pret;
                call pfn;
            }

            if constexpr (std::is_pointer_v<R>) {
                return (R)(uintptr_t)ret;
            }
            else {
                return (R)ret;
            }
        }
    };

    inline function<void*, string> get_function_pointer;
    inline function<size_t, string, real, real, real, real, real> sprite_add;
    inline function<void, real> sprite_delete;
    inline function<void, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real> draw_sprite_general;

    inline void init() noexcept {
        get_function_pointer = (void*)0x0064c89c;
        sprite_add = get_function_pointer("sprite_add");
        sprite_delete = get_function_pointer("sprite_delete");
        draw_sprite_general = get_function_pointer("draw_sprite_general");
    }

}