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
            gm::api::ValueType _type{ gm::api::ValueType::real };
            gm::api::Real _real{};
            gm::api::String _string{};

        public:
            Value() noexcept = default;

            Value(gm::api::Real real) noexcept :
                _real{ real } {}

            Value(gm::api::String string) noexcept :
                _type{ gm::api::ValueType::string },
                _string{ string } {}

            operator gm::api::Real() const noexcept {
                assert(_type == gm::api::ValueType::real);
                return _real;
            }

            operator gm::api::String() const noexcept {
                assert(_type == gm::api::ValueType::string);
                return _string;
            }

            gm::api::ValueType type() const noexcept {
                return _type;
            }
        };

    }

}