#pragma once

#include <cassert>
#include <new>

namespace gm {

    using real = double;
    using string = const char*;

    class var {
        bool _is_string;
        real _real;
        string _string;

    public:
        var() noexcept :
            _is_string{},
            _real{},
            _string{} {}

        var(real num) noexcept :
            _is_string{},
            _real{ num },
            _string{} {}

        var(string str) :
            _is_string{ true },
            _real{} {
            
            assert(str != nullptr);

            size_t len{ strlen(str) };
            char* pstr{ new char[len + 13] };
            new(pstr) uint64_t{};
            new(pstr + 8) uint32_t{ len };
            memcpy(pstr + 12, str, len + 1);
            _string = pstr + 12;
        }

        ~var() noexcept {
            if (_is_string) {
                delete[](_string - 12);
            }
        }

        operator real() const noexcept {
            return _real;
        }

        operator string() const noexcept {
            return _string;
        }
    };

    template<class R, class... Args>
    class function {
        void* _ptr;

    public:
        function(void* ptr = nullptr) noexcept :
            _ptr{ ptr } {}

        R operator()(Args... args) const {
            assert(_ptr != nullptr);

            var arg[]{ args... }, * parg{ arg };
            constexpr uint32_t count{ sizeof...(Args) };
            var ret, * pret{ &ret };
            void* pfn{ _ptr };
            __asm {
                push parg;
                push count;
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