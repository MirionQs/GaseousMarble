#pragma once

#include "type.h"

#include <cassert>
#include <utility>

namespace gm {

    namespace api {

        class value {
            bool _is_string;
            gm::api::real _real;
            gm::api::string _string;

        public:
            value() noexcept :
                _is_string{},
                _real{},
                _string{} {}

            value(gm::api::real num) noexcept :
                _is_string{},
                _real{ num },
                _string{} {}

            value(gm::api::string str) noexcept :
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
                    std::size_t len{ std::strlen(other._string - 12) };
                    char* str{ new char[len + 1] };
                    std::memcpy(str, other._string - 12, len + 1);
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

            operator gm::api::real() const noexcept {
                assert(!_is_string);
                return _real;
            }

            operator gm::api::string() const noexcept {
                assert(_is_string);
                return _string;
            }
        };

    }

}