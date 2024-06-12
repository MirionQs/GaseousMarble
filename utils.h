#pragma once

#include "type.h"

#include <string>

namespace gm {

	std::u16string utf16_base64_decode(string s) {
		static constexpr byte tab[]{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
			0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0
		};

		std::u16string res;
		while (*s != 0) {
			res.push_back(tab[s[0]] << 10 | tab[s[1]] << 4 | tab[s[2]] >> 2);
			if (s[4] == 0) {
				break;
			}
			res.push_back(tab[s[2]] << 14 | tab[s[3]] << 8 | tab[s[4]] << 2 | tab[s[5]] >> 4);
			if (s[6] == '=') {
				break;
			}
			res.push_back(tab[s[5]] << 12 | tab[s[6]] << 6 | tab[s[7]]);
			s += 8;
		}
		return res;
	}

	double pt_to_px(double pt) {
		return pt / 3 * 4;
	}

}