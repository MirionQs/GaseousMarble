#pragma once

#include "value.h"

namespace gm {

    template<class R, class... Args>
    class function {
        void* _ptr;

    public:
        function(void* ptr = nullptr) noexcept :
            _ptr{ ptr } {}

        R operator()(Args... args) const noexcept {
            assert(_ptr != nullptr);

            gm::value args_wrapped[]{ args... }, ret;

            gm::value* argv{ args_wrapped };
            constexpr std::uint32_t argc{ sizeof...(args) };
            gm::value* pret{ &ret };
            void* pfn{ _ptr };
            __asm {
                push argv;
                push argc;
                push pret;
                call pfn;
            }

            if constexpr (std::is_pointer_v<R>) {
                return reinterpret_cast<R>(static_cast<std::uintptr_t>(ret));
            }
            else {
                return static_cast<R>(ret);
            }
        }
    };

}