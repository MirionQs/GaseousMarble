#pragma once

#include <string_view>

namespace gm {

    namespace api {

        class String {
            struct Header {
                gm::u16 code_page;
                gm::u16 char_size;
                gm::u32 ref_count;
                gm::u32 length;
            };

            char* _data;

            decltype(auto) _header(this auto& self) noexcept {
                return *reinterpret_cast<Header*>(self._data - sizeof(Header));
            }

        public:
            String() noexcept :
                String{ "" } {}

            String(std::string_view string) noexcept :
                String{ string.data(), string.length() } {}

            String(const char* string, gm::usize length) noexcept :
                _data{ new char[length + sizeof(Header) + 1] + sizeof(Header) } {
                _header() = { 65001, 1, 0, length };
                std::memcpy(_data, string, length + 1);
            }

            String(const String& other) noexcept :
                String{ other._data, other.length() } {}

            String(String&& other) noexcept :
                _data{ std::exchange(other._data, nullptr) } {}

            ~String() noexcept {
                if (_header().ref_count != 0) { // avoid releasing external strings
                    return;
                }
                delete[](_data - sizeof(Header));
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
                return _header().length;
            }

            const char* data() const noexcept {
                return _data;
            }
        };

    }

}