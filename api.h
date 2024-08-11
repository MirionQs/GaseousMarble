#pragma once

#include "base.h"

namespace gm {

	struct {
		function<void*, string> get_function_pointer;
		function<size_t, string, real, real, real, real, real> sprite_add;
		function<void, real> sprite_delete;
		function<void, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real> draw_sprite_general;

		void init() {
			get_function_pointer = (void*)0x0064c89c;
			sprite_add = get_function_pointer("sprite_add");
			sprite_delete = get_function_pointer("sprite_delete");
			draw_sprite_general = get_function_pointer("draw_sprite_general");
		}
	} api;

}