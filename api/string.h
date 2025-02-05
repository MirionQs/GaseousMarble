#pragma once

#include "../utils/integer.h"

#include <string_view>

namespace gm {

    namespace api {

        struct StringHeader {
            gm::u16 code_page;
            gm::u16 char_size;
            gm::u32 ref_count;
            gm::u32 size;
        };

        class String {
            char* _data;

            gm::api::StringHeader& _header() noexcept {
                return *reinterpret_cast<gm::api::StringHeader*>(_data - sizeof(gm::api::StringHeader));
            }

            const gm::api::StringHeader& _header() const noexcept {
                return *reinterpret_cast<const gm::api::StringHeader*>(_data - sizeof(gm::api::StringHeader));
            }

        public:
            String() noexcept {
                static String empty_string{ "" };
                _data = empty_string._data;
                ++_header().ref_count;
            }

            String(std::string_view string) noexcept :
                _data{ new char[sizeof(gm::api::StringHeader) + string.size() + 1] + sizeof(gm::api::StringHeader) } {

                _header() = { 65001, 1, 1, string.size() };
                std::memcpy(_data, string.data(), string.size() + 1);
            }

            String(const String& other) noexcept :
                _data{ other._data } {

                ++_header().ref_count;
            }

            ~String() noexcept {
                if (--_header().ref_count == 0) {
                    delete[](_data - sizeof(gm::api::StringHeader));
                }
            }

            String& operator=(const String& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                String temp{ other };
                std::swap(_data, temp._data);
                return *this;
            }

            operator std::string_view() const noexcept {
                return { _data, _header().size };
            }

            gm::u32 size() const noexcept {
                return _header().size;
            }

            gm::u32 ref_count() const noexcept {
                return _header().ref_count;
            }

            const char* data() const noexcept {
                return _data;
            }
        };

        class StringView {
            const char* _data;

            const gm::api::StringHeader& _header() const noexcept {
                return *reinterpret_cast<const gm::api::StringHeader*>(_data - sizeof(gm::api::StringHeader));
            }

        public:
            StringView(gm::api::String string = {}) noexcept :
                _data{ string.data() } {}

            operator std::string_view() const noexcept {
                return { _data, _header().size };
            }

            gm::u32 size() const noexcept {
                return _header().size;
            }

            gm::u32 ref_count() const noexcept {
                return _header().ref_count;
            }

            const char* data() const noexcept {
                return _data;
            }
        };

    }

}