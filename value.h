#pragma once

#include "type.h"

#include <cassert>
#include <utility>

namespace gm {

    namespace api {

        enum class ValueType {
            real,
            string
        };

        class Value {
            ValueType _type{ ValueType::real };
            gm::api::Real _real{ 0. };
            gm::api::String _string{ nullptr };

        public:
            Value() noexcept = default;

            Value(gm::api::Real num) noexcept :
                _real{ num } {}

            Value(gm::api::String str) noexcept :
                _type{ ValueType::string } {

                assert(str != nullptr);

                std::size_t len{ std::strlen(str) };
                char* pascal_str{ new char[len + 13] };
                new(pascal_str) std::uint64_t{};
                new(pascal_str + 8) std::size_t{ len };
                std::memcpy(pascal_str + 12, str, len + 1);

                _string = pascal_str + 12;
            }

            Value(const Value& other) noexcept {
                *this = other;
            };

            Value(Value&& other) noexcept {
                *this = std::move(other);
            };

            ~Value() noexcept {
                if (_type == ValueType::string) {
                    delete[](_string - 12);
                }
            }

            Value& operator=(const Value& other) noexcept {
                if (_type == ValueType::string) {
                    delete[](_string - 12);
                }

                _type = other._type;
                if (_type == ValueType::real) {
                    _real = other._real;
                }
                else {
                    std::size_t len{ *reinterpret_cast<const std::size_t*>(other._string - 4) };
                    char* str{ new char[len + 1] };
                    std::memcpy(str, other._string - 12, len + 1);

                    _string = str + 12;
                }

                return *this;
            }

            Value& operator=(Value&& other) noexcept {
                _type = other._type;
                _real = other._real;
                _string = std::exchange(other._string, nullptr);
                return *this;
            }

            operator gm::api::Real() const noexcept {
                assert(_type == ValueType::real);
                return _real;
            }

            operator gm::api::String() const noexcept {
                assert(_type == ValueType::string);
                return _string;
            }

            ValueType type() const noexcept {
                return _type;
            }
        };

    }

}