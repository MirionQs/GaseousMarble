#pragma once

#include <string>

namespace gm {

	inline std::wstring utf8_to_ucs2(std::string_view str) {
		std::wstring ret;
		auto p{ str.begin() }, end{ str.end() };
		while (p != end) {
			if ((*p & 0x80) == 0) {
				ret += p[0];
				p += 1;
			}
			else if ((*p & 0xe0) == 0xc0) {
				ret += (p[0] & 0x1f) << 6 | p[1] & 0x3f;
				p += 2;
			}
			else if ((*p & 0xf0) == 0xe0) {
				ret += p[0] << 12 | (p[1] & 0x3f) << 6 | p[2] & 0x3f;
				p += 3;
			}
		}
		return ret;
	}

}