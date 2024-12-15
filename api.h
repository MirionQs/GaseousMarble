#pragma once

#include <cstring>
#include <cstdint>
#include <new>

namespace gm {

	using real = double;
	using string = const char*;

	class var {
		bool _isString;
		real _real;
		string _string;

	public:
		var(real r = 0) {
			_isString = false;
			_real = r;
			_string = nullptr;
		}

		var(string s) {
			if (s == nullptr) {
				s = "";
			}

			size_t size{ strlen(s) };
			char* data{ new char[size + 13] };
			new(data) uint64_t{};
			new(data + 8) uint32_t{ size };
			memcpy(data + 12, s, size + 1);

			_isString = true;
			_real = 0;
			_string = data + 12;
		}

		~var() {
			if (_isString) {
				delete[](_string - 12);
			}
		}

		operator real() const {
			return _real;
		}

		operator string() const {
			return _string;
		}
	};

	template<class R, class... Args>
	class function {
		void* _ptr;

	public:
		function(void* p = nullptr) {
			_ptr = p;
		}

		R operator()(Args... a) const {
			if (_ptr == nullptr) {
				return R{};
			}

			var args[]{ a... }, * pargs{ args };
			constexpr uint32_t count{ sizeof...(Args) };
			var ret, * pret{ &ret };
			void* pfn{ _ptr };
			__asm {
				push pargs;
				push count;
				push pret;
				call pfn;
			}

			if constexpr (std::is_pointer_v<R>) {
				return (R)(uintptr_t)ret;
			}
			else {
				return (R)ret;
			}
		}
	};

	inline function<void*, string> get_function_pointer;
	inline function<size_t, string, real, real, real, real, real> sprite_add;
	inline function<void, real> sprite_delete;
	inline function<void, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real, real> draw_sprite_general;

	inline void init() {
		get_function_pointer = (void*)0x0064c89c;
		sprite_add = get_function_pointer("sprite_add");
		sprite_delete = get_function_pointer("sprite_delete");
		draw_sprite_general = get_function_pointer("draw_sprite_general");
	}

}