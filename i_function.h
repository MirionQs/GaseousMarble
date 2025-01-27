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

            template<class R, class... Args>
            R _invoke(FunctionId id, Args... args) const noexcept {
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

        public:
            IFunction() noexcept :
                _resource{ reinterpret_cast<FunctionResource*>(0x00686b1c) } {};

            void draw_sprite_general(
                std::size_t sprite_id,
                std::int32_t image_index,
                std::uint32_t image_x, std::uint32_t image_y,
                std::size_t width, std::size_t height,
                double x, double y,
                double scale_x, double scale_y,
                double rotate,
                std::uint32_t color1, std::uint32_t color2, std::uint32_t color3, std::uint32_t color4,
                double alpha
            ) const noexcept {
                _invoke<void,
                    gm::api::Real,
                    gm::api::Real,
                    gm::api::Real, gm::api::Real,
                    gm::api::Real, gm::api::Real,
                    gm::api::Real, gm::api::Real,
                    gm::api::Real, gm::api::Real,
                    gm::api::Real,
                    gm::api::Real, gm::api::Real, gm::api::Real, gm::api::Real,
                    gm::api::Real
                >(FunctionId::draw_sprite_general,
                    sprite_id,
                    image_index,
                    image_x, image_y,
                    width, height,
                    x, y,
                    scale_x, scale_y,
                    rotate,
                    color1, color2, color3, color4,
                    alpha
                );
            }

            std::size_t sprite_add(
                std::string_view path,
                std::size_t image_count,
                bool remove_background,
                bool smooth_edges,
                std::int32_t origin_x, std::int32_t origin_y
            ) const noexcept {
                return _invoke<std::size_t,
                    gm::api::String,
                    gm::api::Real,
                    gm::api::Real,
                    gm::api::Real,
                    gm::api::Real, gm::api::Real
                >(FunctionId::sprite_add,
                    path.data(),
                    image_count,
                    remove_background,
                    smooth_edges,
                    origin_x, origin_y
                );
            }

            void sprite_delete(std::size_t sprite_id) const noexcept {
                _invoke<void, gm::api::Real>(FunctionId::sprite_delete, sprite_id);
            }
        };

        inline gm::api::IFunction function;

    }

}