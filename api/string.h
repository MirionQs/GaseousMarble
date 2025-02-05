#pragma once

#include <string_view>

namespace gm {

    namespace api {

        struct StringHeader {
            gm::u16 code_page;
            gm::u16 char_size;
            gm::u32 ref_count;
            gm::u32 length;
        };

        class String {
            char* _data;

            decltype(auto) _header(this auto& self) noexcept {
                return *reinterpret_cast<StringHeader*>(self._data - sizeof(StringHeader));
            }

        public:
            String() noexcept :
                String{ "" } {}

            String(std::string_view string) noexcept :
                _data{ new char[sizeof(StringHeader) + string.length() + 1] + sizeof(StringHeader) } {

                _header() = { 65001, 1, 0, string.length() };
                std::memcpy(_data, string.data(), string.length() + 1);
            }

            String(const String& other) noexcept :
                String{ { other._data, other.length() } } {}

            String(String&& other) noexcept :
                _data{ std::exchange(other._data, nullptr) } {}

            ~String() noexcept {
                if (_header().ref_count != 0) { // avoid releasing external strings
                    return;
                }
                delete[](_data - sizeof(StringHeader));
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

            gm::u32 use_count() const noexcept {
                return _header().ref_count;
            }

            const char* data() const noexcept {
                return _data;
            }
        };

        class StringView {
            const char* _data;

            const StringHeader& _header() const noexcept {
                return *reinterpret_cast<const StringHeader*>(_data - sizeof(StringHeader));
            }

        public:
            StringView(gm::api::String string) noexcept :
                _data{ string.data() } {}

            operator std::string_view() const noexcept {
                return { _data, length() };
            }

            gm::u32 length() const noexcept {
                return _header().length;
            }

            gm::u32 use_count() const noexcept {
                return _header().ref_count;
            }

            const char* data() const noexcept {
                return _data;
            }
        };

    }

}