#pragma once

#include "type.hpp"

namespace axis::monad {

inline auto Err(Error error) {
  return std::unexpected(error);
}

template <typename T>
auto Ok(T value) -> Result<T> {
  return {std::move(value)};
}

auto Ok() -> Status {
  return unit;
}

}  // namespace axis::monad