#pragma once

#include "type.h"

#include <cassert>
#include <utility>

namespace gm {

    namespace api {

        class Value {
            bool _is_string;
            gm::api::Real _real;
            gm::api::String _string;

        public:
            Value() noexcept :
                _is_string{},
                _real{},
                _string{} {}

            Value(gm::api::Real num) noexcept :
                _is_string{},
                _real{ num },
                _string{} {}

            Value(gm::api::String str) noexcept :
                _is_string{ true },
                _real{} {

                assert(str != nullptr);

                std::size_t len{ std::strlen(str) };
                char* pas_str{ new char[len + 13] };
                new(pas_str) std::uint64_t{};
                new(pas_str + 8) std::uint32_t{ len };
                std::memcpy(pas_str + 12, str, len + 1);

                _string = pas_str + 12;
            }

            Value(const Value& other) noexcept {
                *this = other;
            };

            Value(Value&& other) noexcept {
                *this = std::move(other);
            };

            ~Value() noexcept {
                if (_is_string) {
                    delete[](_string - 12);
                }
            }

            Value& operator=(const Value& other) noexcept {
                if (_is_string) {
                    delete[](_string - 12);
                }

                _is_string = other._is_string;
                if (_is_string) {
                    std::size_t len{ std::strlen(other._string - 12) };
                    char* str{ new char[len + 1] };
                    std::memcpy(str, other._string - 12, len + 1);

                    _string = str + 12;
                }
                else {
                    _real = other._real;
                }

                return *this;
            }

            Value& operator=(Value&& other) noexcept {
                _is_string = other._is_string;
                _real = other._real;
                _string = std::exchange(other._string, nullptr);
                return *this;
            }

            operator gm::api::Real() const noexcept {
                assert(!_is_string);
                return _real;
            }

            operator gm::api::String() const noexcept {
                assert(_is_string);
                return _string;
            }
        };

    }

}