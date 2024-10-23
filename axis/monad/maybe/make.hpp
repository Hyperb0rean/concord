#pragma once

#include "type.hpp"

namespace axis::monad {
template<typename T>
inline auto Just(T v) -> Maybe<T> { // NOLINT
    return {std::move(v)};
}

auto None() -> std::nullopt_t { // NOLINT
    return std::nullopt;
}

} // namespace axis::monad