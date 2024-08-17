#pragma once

#include "type.hpp"

template <typename T, typename C>
auto operator|(::axis::monad::Result<T> r, C c) {
  return c.Pipe(std::move(r));
}