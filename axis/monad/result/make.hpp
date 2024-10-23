#pragma once

#include "type.hpp"

namespace axis::monad {

inline auto Err(Error error) { // NOLINT
    return std::unexpected(error);
}

template<typename T>
auto Ok(T value) -> Result<T> { // NOLINT
    return {std::move(value)};
}

auto Ok() -> Status { // NOLINT
    return unit;
}

} // namespace axis::monad