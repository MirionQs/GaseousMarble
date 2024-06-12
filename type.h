#pragma once

#include <cstring>
#include <cstdint>
#include <new>

namespace gm {

	using byte = std::uint8_t;
	using word = std::uint16_t;
	using dword = std::uint32_t;

	using real = double;
	using string = const char*;

	class var {
		dword _type;
		double _real;
		char* _string;

	public:
		var(real r = 0) {
			_type = 0;
			_real = r;
			_string = nullptr;
		}

		var(string s) {
			_type = 1;
			_real = 0;
			if (s == nullptr) {
				_string = nullptr;
				return;
			}
			dword size{std::strlen(s)};
			char* data{new char[size + 13]};
			new(data) dword(0);
			new(data + 4) dword(0);
			new(data + 8) dword(size);
			std::memcpy(data + 12, s, size + 1);
			_string = data + 12;
		}

		~var() {
			if (_string != nullptr) {
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
			constexpr dword count{sizeof...(a)};
			var ret, * pret{&ret};
			void* pfn{_ptr};
			__asm {
				push pargs;
				push count;
				push pret;
				call pfn;
			}
			if constexpr (::std::is_same_v<R, void*>) {
				return (void*)(dword)ret;
			}
			else {
				return (R)ret;
			}
		}
	};

}