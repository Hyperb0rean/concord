#pragma once

#include "type.hpp"

namespace axis::monad {
template <typename T>
inline auto Just(T v) -> Maybe<T> {
  return {std::move(v)};
}

auto Nothing() -> std::nullopt_t {
  return std::nullopt;
}

}  // namespace axis::monad