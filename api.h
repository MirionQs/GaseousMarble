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
		var(real r = 0);
		var(string s);
		~var();

		operator gm::real() const;
		operator gm::string() const;
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

			var args[]{a...}, * pargs{args};
			constexpr uint32_t count{sizeof...(Args)};
			var ret, * pret{&ret};
			void* pfn{_ptr};
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

	void init();

}