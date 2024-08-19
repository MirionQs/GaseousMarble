#pragma once

#include "base.h"

#include <string>

namespace gm {

	inline std::wstring utf8_to_ucs2(string str) {
		std::wstring res;
		uint8_t* view{(uint8_t*)str};
		while (*view != 0) {
			if (view[0] >> 7 == 0) {
				res.push_back(view[0]);
				view += 1;
			}
			else if (view[0] >> 5 == 6) {
				if (view[1] >> 6 != 2) {
					break;
				}
				res.push_back((view[0] & 31) << 6 | view[1] & 63);
				view += 2;
			}
			else if (view[0] >> 4 == 14) {
				if (view[1] >> 6 != 2 || view[2] >> 6 != 2) {
					break;
				}
				res.push_back(view[0] << 12 | (view[1] & 63) << 6 | view[2] & 63);
				view += 3;
			}
			else {
				break;
			}
		}
		return res;
	}

}