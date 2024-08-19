#include "api.h"

gm::var::var(real r) {
	_isString = false;
	_real = r;
	_string = nullptr;
}

gm::var::var(string s) {
	if (s == nullptr) {
		s = "";
	}

	size_t size{strlen(s)};
	char* data{new char[size + 13]};
	new(data) uint32_t(0);
	new(data + 4) uint32_t(0);
	new(data + 8) uint32_t(size);
	memcpy(data + 12, s, size + 1);

	_isString = true;
	_real = 0;
	_string = data + 12;
}

gm::var::~var() {
	if (_isString) {
		delete[](_string - 12);
	}
}

gm::var::operator gm::real() const {
	return _real;
}

gm::var::operator gm::string() const {
	return _string;
}

void gm::init() {
	get_function_pointer = (void*)0x0064c89c;
	sprite_add = get_function_pointer("sprite_add");
	sprite_delete = get_function_pointer("sprite_delete");
	draw_sprite_general = get_function_pointer("draw_sprite_general");
}