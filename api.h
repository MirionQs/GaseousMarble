#pragma once

#include "function.h"

namespace gm {

    namespace api {

        inline gm::api::Function<void*, gm::api::String>
            get_function_address;
        inline gm::api::Function<std::size_t, gm::api::String, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real>
            sprite_add;
        inline gm::api::Function<void, gm::api::Real>
            sprite_delete;
        inline gm::api::Function<void, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real>
            draw_sprite_general;

        class IFunction {
            struct FunctionData {
                std::uint8_t name_size;
                char name[67];
                void* address;
                std::size_t argument_count;
                bool require_pro;
            };

            struct FunctionResource {
                FunctionData* functions;
                std::size_t size;
            };

            FunctionResource* _resource;

        public:
            enum class FunctionId {
                draw_sprite_general = 141,
                get_function_address = 721,
                sprite_add = 828,
                sprite_delete = 832
            };

            IFunction() noexcept :
                _resource{ reinterpret_cast<FunctionResource*>(0x00686b1c) } {};

            template<class R, class... Args>
            R call(FunctionId id, Args... args) const noexcept {

            }
        };

        inline IFunction function;

        inline void init() noexcept {
            gm::api::get_function_address = reinterpret_cast<void*>(0x0064c89c);
            gm::api::sprite_add = gm::api::get_function_address("sprite_add");
            gm::api::sprite_delete = gm::api::get_function_address("sprite_delete");
            gm::api::draw_sprite_general = gm::api::get_function_address("draw_sprite_general");
        }

    }

}