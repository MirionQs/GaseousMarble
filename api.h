#pragma once

#include <cassert>
#include <utility>

namespace gm {

    using real = double;
    using string = const char*;

    class value {
        bool _is_string;
        gm::real _real;
        gm::string _string;

    public:
        value() noexcept :
            _is_string{},
            _real{},
            _string{} {}

        value(gm::real num) noexcept :
            _is_string{},
            _real{ num },
            _string{} {}

        value(gm::string str) noexcept :
            _is_string{ true },
            _real{} {

            assert(str != nullptr);

            std::size_t len{ strlen(str) };
            char* pas_str{ new char[len + 13] };
            new(pas_str) std::uint64_t{};
            new(pas_str + 8) std::uint32_t{ len };
            memcpy(pas_str + 12, str, len + 1);

            _string = pas_str + 12;
        }

        value(const value& other) noexcept {
            *this = other;
        };

        value(value&& other) noexcept {
            *this = std::move(other);
        };

        ~value() noexcept {
            if (_is_string) {
                delete[](_string - 12);
            }
        }

        value& operator=(const value& other) noexcept {
            _is_string = other._is_string;
            if (_is_string) {
                std::size_t len{ strlen(other._string - 12) };
                char* str{ new char[len + 1] };
                memcpy(str, other._string - 12, len + 1);
                _string = str;
            }
            else {
                _real = other._real;
            }
            return *this;
        }

        value& operator=(value&& other) noexcept {
            _is_string = other._is_string;
            _real = other._real;
            _string = std::exchange(other._string, nullptr);
            return *this;
        }

        operator gm::real() const noexcept {
            assert(!_is_string);
            return _real;
        }

        operator gm::string() const noexcept {
            assert(_is_string);
            return _string;
        }
    };

    template<class R, class... Args>
    class function {
        void* _ptr;

    public:
        function(void* ptr = nullptr) noexcept :
            _ptr{ ptr } {}

        R operator()(Args... args) const noexcept {
            assert(_ptr != nullptr);

            gm::value args_wrapped[]{ args... }, ret;

            gm::value* argv{ args_wrapped };
            constexpr std::uint32_t argc{ sizeof...(args) };
            gm::value* pret{ &ret };
            void* pfn{ _ptr };
            __asm {
                push argv;
                push argc;
                push pret;
                call pfn;
            }

            if constexpr (std::is_pointer_v<R>) {
                return reinterpret_cast<R>(static_cast<std::uintptr_t>(ret));
            }
            else {
                return static_cast<R>(ret);
            }
        }
    };

    inline gm::function<void*, gm::string>
        get_function_address;
    inline gm::function<std::uint32_t, gm::string, gm::real, gm::real, gm::real, gm::real, gm::real>
        sprite_add;
    inline gm::function<void, gm::real>
        sprite_delete;
    inline gm::function<void, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real, gm::real>
        draw_sprite_general;

    inline void init() noexcept {
        gm::get_function_address = reinterpret_cast<void*>(0x0064c89c);
        gm::sprite_add = gm::get_function_address("sprite_add");
        gm::sprite_delete = gm::get_function_address("sprite_delete");
        gm::draw_sprite_general = gm::get_function_address("draw_sprite_general");
    }

}