#pragma once

#include "value.h"

#include <string>

namespace gm {

    namespace api {

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

            enum class FunctionId {
                draw_sprite_general = 141,
                sprite_add = 828,
                sprite_delete = 832
            };

            FunctionResource* _resource;

        public:
            IFunction() noexcept :
                _resource{ reinterpret_cast<FunctionResource*>(0x00686b1c) } {};

            template<class R, class... Args>
            R call(FunctionId id, Args... args) const noexcept {
                gm::api::Value args_wrapped[]{ args... }, ret;

                gm::api::Value* argv{ args_wrapped };
                constexpr std::size_t argc{ sizeof...(args) };
                gm::api::Value* pret{ &ret };
                void* pfn{ _resource->functions[static_cast<std::size_t>(id)].address };
                __asm {
                    push argv;
                    push argc;
                    push pret;
                    call pfn;
                }

                return static_cast<R>(ret);
            }

            void draw_sprite_general(std::size_t sprite_id, std::size_t image_index, double x, double y, std::size_t width, std::size_t height, double offset_x, double offset_y, double scale_x, double scale_y, double rotate, std::uint32_t color1, std::uint32_t color2, std::uint32_t color3, std::uint32_t color4, double alpha) const noexcept {
                call<void, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real>(FunctionId::draw_sprite_general, sprite_id, image_index, x, y, width, height, offset_x, offset_y, scale_x, scale_y, rotate, color1, color2, color3, color4, alpha);
            }

            std::size_t sprite_add(std::string_view filename, std::size_t image_number, bool remove_background, bool smooth, std::int32_t x, std::int32_t y) const noexcept {
                return call<std::size_t, gm::api::String, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real>(FunctionId::sprite_add, filename.data(), image_number, remove_background, smooth, x, y);
            }

            void sprite_delete(std::size_t sprite_id) const noexcept {
                call<void, gm::api::Real>(FunctionId::sprite_delete, sprite_id);
            }
        };

        inline gm::api::IFunction function;

    }

}