#pragma once

#include "utils/int.h"

#include <string_view>

namespace gm {

    namespace api {

        using Real = gm::f64;

        class String {
            const char* _data;

        public:
            String() noexcept :
                String{ "" } {}

            String(std::string_view string) noexcept :
                String{ string.data(), string.length() } {}

            String(const char* string, gm::usize length) noexcept {
                char* delphi{ new char[length + 13] + 12 };
                new(delphi - 12) gm::u16{ 65001 };  // code page
                new(delphi - 10) gm::u16{ 1 };      // char size
                new(delphi - 8) gm::u32{};          // ref count
                new(delphi - 4) gm::u32{ length };
                std::memcpy(delphi, string, length + 1);
                _data = delphi;
            }

            String(const String& other) noexcept :
                String{ other._data, other.length() } {}

            String(String&& other) noexcept :
                _data{ std::exchange(other._data, nullptr) } {}

            ~String() noexcept {
                if (ref_count() != 0) {
                    return;
                }
                delete[](_data - 12);
            }

            String& operator=(const String& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                String temp{ other };
                std::swap(*this, temp);
                return *this;
            }

            String& operator=(String&& other) noexcept {
                String temp{ std::move(other) };
                std::swap(_data, temp._data);
                return *this;
            }

            operator std::string_view() const noexcept {
                return { _data, length() };
            }

            gm::u32 length() const noexcept {
                return *reinterpret_cast<const gm::u32*>(_data - 4);
            }

            gm::u32 ref_count() const noexcept {
                return *reinterpret_cast<const gm::u32*>(_data - 8);
            }

            const char* data() const noexcept {
                return _data;
            }
        };

    }

}