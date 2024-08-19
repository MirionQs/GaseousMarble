#pragma once

#include <cstring>
#include <cstdint>
#include <new>

namespace gm {

	using real = double;
	using string = const char*;

	class var {
		bool _type;
		real _real;
		string _string;

	public:
		var(real r = 0) {
			_type = 0;
			_real = r;
			_string = nullptr;
		}

		var(string s) {
			if (s == nullptr) {
				s = "";
			}

			size_t size{strlen(s)};
			char* data{new char[size + 13]};
			new(data) uint32_t(0);
			new(data + 4) uint32_t(0);
			new(data + 8) uint32_t(size);
			memcpy(data + 12, s, size + 1);

			_type = 1;
			_real = 0;
			_string = data + 12;
		}

		~var() {
			if (_type == 1) {
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

			var args[]{a...}, * pargs{args};
			constexpr uint32_t count{sizeof...(a)};
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

	inline function<void*, string> get_function_pointer{(void*)0x0064c89c};

}