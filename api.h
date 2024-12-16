#pragma once

#include <stdexcept>

namespace gm {

    using real = double;
    using string = const char*;

    class var {
        bool _isString;
        real _real;
        string _string;

    public:
        var(real num = 0) noexcept {
            _isString = false;
            _real = num;
            _string = nullptr;
        }

        var(string str) {
            if (str == nullptr) {
                throw std::invalid_argument{ "Cannot initialize var with nullptr." };
            }

            size_t size{ strlen(str) };
            char* data{ new char[size + 13] };
            new(data) uint64_t{};
            new(data + 8) uint32_t{ size };
            memcpy(data + 12, str, size + 1);

            _isString = true;
            _real = 0;
            _string = data + 12;
        }

        ~var() noexcept {
            if (_isString) {
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
        function(void* ptr = nullptr) noexcept {
            _ptr = ptr;
        }

        R operator()(Args... args) const {
            if (_ptr == nullptr) {
                throw std::runtime_error{ "Cannot call nullptr." };
            }

            var arg[]{ args... }, * parg{ args };
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