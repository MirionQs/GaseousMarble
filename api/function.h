#pragma once

#include "value.h"

namespace gm {

    namespace api {

        class Function {
            gm::u8 _name_length;
            char _name[67];
            void* _address;
            gm::u32 _arg_count;
            bool _require_pro;

        public:
            Function() = delete;
            Function(const Function&) = delete;
            Function(Function&&) = delete;

            Function& operator=(const Function&) = delete;
            Function& operator=(Function&&) = delete;

            std::string_view name() const noexcept {
                return { _name, _name_length };
            }

            gm::u32 arg_count() const noexcept {
                return _arg_count;
            }

            template<class R, class... Args>
            R call(Args... args) const noexcept {
                gm::api::Value args_wrapped[]{ args... }, returned;
                gm::api::Value* argv{ args_wrapped };
                constexpr gm::u32 argc{ sizeof...(args) };
                gm::api::Value* pret{ &returned };
                void* pfn{ _address };

                // This assertion will fail when quitting the game because the
                // resources have already been released by GameMaker.
                assert(argc == _arg_count);

                __asm {
                    push argv;
                    push argc;
                    push pret;
                    call pfn;
                }

                return static_cast<R>(returned);
            }
        };

    }

}