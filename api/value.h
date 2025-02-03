#pragma once

#include "type.h"

#include <cassert>

namespace gm {

    namespace api {

        enum class ValueType {
            real,
            string
        };

        class Value {
            ValueType _type{ ValueType::real };
            gm::api::Real _real{};
            gm::api::String _string{};

        public:
            Value() noexcept = default;

            Value(gm::api::Real real) noexcept :
                _real{ real } {}

            Value(gm::api::String string) noexcept :
                _type{ ValueType::string },
                _string{ string } {}

            operator gm::api::Real() const noexcept {
                assert(_type == ValueType::real);
                return _real;
            }

            operator gm::api::String() const noexcept {
                assert(_type == ValueType::string);
                return _string;
            }

            ValueType type() const noexcept {
                return _type;
            }
        };

    }

}