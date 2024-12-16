#pragma once

#include <string>

namespace gm {

    inline std::wstring utf8_to_ucs2(std::string_view str) noexcept {
        std::wstring ret;
        auto i{ str.begin() }, end{ str.end() };
        while (i != end) {
            if ((*i & 0x80) == 0) {
                ret += i[0];
                i += 1;
            }
            else if ((*i & 0xe0) == 0xc0) {
                ret += (i[0] & 0x1f) << 6 | i[1] & 0x3f;
                i += 2;
            }
            else if ((*i & 0xf0) == 0xe0) {
                ret += i[0] << 12 | (i[1] & 0x3f) << 6 | i[2] & 0x3f;
                i += 3;
            }
        }
        return ret;
    }

}